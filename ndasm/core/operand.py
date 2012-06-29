import re

class InvalidOperandException(Exception):
	"""
	Invalid operand exception
	"""
	def __init__(self, operand):
		Exception.__init__(self)
		self.operand = operand
	def __str__(self):
		return self.operand.getRaw()

class Operand:
	"""
	Default operand class
	"""	
	
	OP_UNK,OP_IMM,OP_IMM08,OP_IMM16,OP_REG16,OP_IND_REG16=-1,0,1,2,3,4	
	SIZES = [None,1,2,1,1]
	
	def __init__(self, raw):
		self.__raw = raw
		self.__optype = Operand.OP_UNK
		self.__value = None
		self.is_reference = False
		if re.match('^:[@a-z0-9_]+$',self.__raw,re.I):
			self.__optype = Operand.OP_IMM16
			self.is_reference = True
		self.__detect()

	def getSize(self):
		if self.isReference():
			return Operand.SIZES[Operand.OP_IMM16]
		else:
			return Operand.SIZES[self.getType()]
		
	def getRaw(self):
		"""
		Get raw operand (plaintext)
		"""
		return self.__raw

	def getValue(self):
		return self.__value

	def getType(self):
		return self.__optype
	
	def isReference(self):
		return self.is_reference

	def toImm08(self):
		"""
		Try to convert to imm08 if possible. If not, raise InvalidOperandException
		"""
		# normally, raw value cannot be a reference
		# so we can only check raw value matches imm08 syntax and value
		r = re.match('^#(x?[0-9A-F]+)$',self.__raw,re.I)
		if r:
			value = r.group(1)
			# check if this value is hexa
			if value[0]=='x' or value[0]=='X':
				try:
					v = int(value[1:],16)
					if v>=0 and v<=0xFF:
						self.__value = v
						self.__optype = Operand.OP_IMM08
					else:
						raise InvalidOperandException(self)
				except:
					raise InvalidOperandException(self)
			else:
				try:
					v = int(value,10)
					if v>=0 and v<=0xFF:
						self.__value = v
						self.__optype = Operand.OP_IMM08
					else:
						raise InvalidOperandException(self)
				except:
					raise InvalidOperandException(self)
		else:
			raise InvalidOperandException(self)
			
	def toImm16(self):
		"""
		Try to convert to imm08 if possible. If not, raise InvalidOperandException
		"""
		# normally, raw value cannot be a reference
		# so we can only check raw value matches imm08 syntax and value
		if not self.isReference():
			r = re.match('^#(x?[0-9A-F]+)$',self.__raw,re.I)
			if r:
				value = r.group(1)
				# check if this value is hexa
				if value[0]=='x' or value[0]=='X':
					try:
						v = int(value[1:],16)
						if v>=0 and v<=0xFFFF:
							self.__value = v
							self.__optype = Operand.OP_IMM16
						else:
							raise InvalidOperandException(self)
					except:
						raise InvalidOperandException(self)
				else:
					try:
						v = int(value)
						if v>=0 and v<=0xFFFF:
							self.__value = v
							self.__optype = Operand.OP_IMM16
						else:
							raise InvalidOperandException(self)
					except:
						raise InvalidOperandException(self)
			else:
				raise InvalidOperandException(self)

	def toReg16(self):
		"""
		Try to convert to reg16 if possible. If not, raise InvalidOperandException
		"""
		r = re.match('^(r[0-7]+|sp|bp)$',self.__raw,re.I)
		if r:
			reg = r.group(1)
			if reg.lower()=='sp':
				self.__value = 8
				self.__optype = Operand.OP_REG16
			elif reg.lower()=='bp':
				self.__value = 9
				self.__optype = Operand.OP_REG16
			else:
				try:
					r = int(reg[1:])
					if r>=0 and r<=7:
						self.__value = r
						self.__optype = Operand.OP_REG16
					else:
						raise InvalidOperandException(self)
				except:
					raise InvalidOperandException(self)
		else:
			raise InvalidOperandException(self)
			
	def toIndReg16(self):
		"""
		Try to convert to reg16 if possible. If not, raise InvalidOperandException
		"""
		r = re.match('^\[\s*r([0-7]+|sp|bp)\s*\]$',self.__raw,re.I)
		if r:
			reg = r.group(1)
			if reg.lower()=='sp':
				self.__value = 8
				self.__optype = Operand.OP_IND_REG16
			elif reg.lower()=='bp':
				self.__value = 9
				self.__optype = Operand.OP_IND_REG16
			else:
				try:
					r = int(reg)
					if r>=0 and r<=7:
						self.__value = r
						self.__optype = Operand.OP_IND_REG16
					else:
						raise InvalidOperandException(self)
				except:
					raise InvalidOperandException(self)
		else:
			raise InvalidOperandException(self)

	def updateReference(self, reference, value):
		if self.isReference():
			if self.__raw == reference:
				self.__raw = value
				self.toImm16()
				self.is_reference = False

	def __detect(self):
		if not self.isReference():
			makers = [self.toReg16, self.toIndReg16,self.toImm08,self.toImm16]
			for maker in makers:
				try:
					maker()
					return
				except InvalidOperandException,e:
					pass
			raise InvalidOperandException(e.operand)
		else:
			self.__optype = Operand.OP_IMM16

	def isReference(self):
		return self.__raw[0]==':'

	def __emitBytes(self, values):
		#print values
		return ''.join([chr(c) for c in values])
		

	def toBytes(self):
		if self.getType()==Operand.OP_REG16 or self.getType()==Operand.OP_IND_REG16:
			return self.__emitBytes([self.__value])
		elif self.getType()==Operand.OP_IMM08:
			return self.__emitBytes([self.__value])
		elif self.getType()==Operand.OP_IMM16:
			return self.__emitBytes([self.__value&0xFF,self.__value>>8])
