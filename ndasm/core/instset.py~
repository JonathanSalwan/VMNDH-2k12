from operand import *

OP_PUSH = 0x01
OP_POP = 0x03
OP_MOV = 0x04
OP_STRCPY = 0x05
OP_ADD = 0x06
OP_SUB = 0x07
OP_MUL = 0x08
OP_DIV = 0x09
OP_INC = 0x0A
OP_DEC = 0x0B
OP_OR = 0x0C
OP_AND = 0x0D
OP_XOR = 0x0E
OP_NOT = 0x0F
OP_JZ = 0x10
OP_JNZ = 0x11
OP_JMPS = 0x16
OP_TEST = 0x17
OP_CMP = 0x18
OP_CALL = 0x19
OP_RET = 0x1A
OP_JMPL = 0x1B
OP_END = 0x1C
OP_XCHG = 0x1D
OP_JA = 0x1E
OP_JB = 0x1F
OP_SYSCALL = 0x30
OP_NOP = 0x02


class InvalidInstException(Exception):
	def __init__(self):
		Exception.__init__(self)

class MethodMustBeImplemented(Exception):
	def __init__(self):
		Exception.__init__(self)

class Inst:

	"""
	Default instruction
	"""
	
	def __init__(self, line, operands=[]):
		self.line = line
		self.operands = operands
		self.offset = None
		
	def containsReference(self):
		for operand in self.operands:
			if operand.isReference():
				return True
		return False
	
	def setOffset(self, offset):
		self.offset = offset
		
	def getOffset(self):
		return self.offset
	
	def updateReference(self, ref, value):
		for operand in self.operands:
			if operand.isReference():
				operand.updateReference(ref, value)

	def hasUnresolvedRefs(self):
		unref = []
		for operand in self.operands:
			if operand.isReference():
				unref.append(operand.getRaw())
		return ','.join(unref)

	def getOpSize(self):
		t = 0
		for o in self.operands:
			t += o.getSize()
		return t
		
	def getSize(self):
		raise MethodMustBeImplemented()

	def toBytes(self):
		raise MethodMustBeImplemented()

class NakedInst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode
		if len(operands)!=0:
			raise InvalidInstException()
		
	def getSize(self):
		return 1
		
	def toBytes(self):
		return '%c' % self.opcode

class RegInst(Inst):
	def __init__(self, line, opcode, operands=[],flag=False):
		Inst.__init__(self, line, operands)
		self.opcode = opcode
		self.flag = flag
		if len(operands)!=1:
			raise InvalidInstException()
		if operands[0].getType()!=Operand.OP_REG16:
			raise InvalidInstException()
	
	def getSize(self):
		if self.flag:
			return 3
		else:
			return 2
		
	def toBytes(self):
		s = '%c'%self.opcode
		if self.flag:
			s += '\x03'
		s += self.operands[0].toBytes()
		return s
		
class Imm8Inst(Inst):
	def __init__(self, line, opcode, operands=[], flag=False):
		Inst.__init__(self, line, operands)
		self.opcode = opcode
		self.flag = flag
		if len(operands)!=1:
			raise InvalidInstException()
		if operands[0].getType()!=Operand.OP_IMM08:
			raise InvalidInstException()
	
	def getSize(self):
		if self.flag:
			return 3
		else:
			return 2
		
	def toBytes(self):
		s = '%c' % self.opcode
		if self.flag:
			s += '\x05'
		s += self.operands[0].toBytes()
		return s

class Imm16Inst(Inst):
	def __init__(self, line, opcode, operands=[],flag=False):
		Inst.__init__(self, line, operands)
		self.opcode = opcode
		self.flag = flag
		if len(operands)!=1:
			raise InvalidInstException()
		if operands[0].getType()!=Operand.OP_IMM08 and operands[0].getType()!=Operand.OP_IMM16:
			raise InvalidInstException()
		else:
			self.operands[0].toImm16()
	
	def getSize(self):
		if self.flag:
			return 4
		else:
			return 3
		
	def toBytes(self):
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += self.operands[0].toBytes()
		return s
	

class RegRegInst(Inst):
	def __init__(self, line, opcode, operands=[],flag=True):
		Inst.__init__(self, line, operands)
		self.opcode = opcode
		self.flag = flag
		
		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_REG16 or operands[1].getType()!=Operand.OP_REG16:
			raise InvalidInstException()
		 
	def getSize(self):
		if self.flag:
			return 2 + self.getOpSize()
		else:
			return 1 + self.getOpSize()
						
	def toBytes(self):
		s = "%c"%self.opcode
		if self.flag:
			s+= "\x00"
		s += self.operands[0].toBytes()+self.operands[1].toBytes()
		return s

class RegImm8Inst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		if operands[0].getType()!=Operand.OP_REG16 or operands[1].getType()!=Operand.OP_IMM08:
			#print operands[1].getType()
			raise InvalidInstException()
		 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x01"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()
		
class RegImm16Inst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()==Operand.OP_REG16 and (operands[1].getType()==Operand.OP_IMM08 or operands[1].getType()==Operand.OP_IMM16):
			self.operands[1].toImm16()
		else:
			raise InvalidInstException()
				 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x02"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()

class RegIndInst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_REG16 or operands[1].getType()!=Operand.OP_IND_REG16:
			raise InvalidInstException()
		 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x0A"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()

class IndRegInst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_IND_REG16 or operands[1].getType()!=Operand.OP_REG16:
			raise InvalidInstException()
		 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x06"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()
	
class IndImm8Inst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_IND_REG16 or operands[1].getType()!=Operand.OP_IMM08:
			raise InvalidInstException()
		 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x07"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()

class IndImm16Inst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_IND_REG16 and (operands[1].getType()!=Operand.OP_IMM08 and operands[1].getType()!=Operand.OP_IMM16):
			raise InvalidInstException() 
		else:
			self.operands[1].toImm16()
					 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x08"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()

class IndIndInst(Inst):
	def __init__(self, line, opcode, operands=[]):
		Inst.__init__(self, line, operands)
		self.opcode = opcode

		# 2 operands are mandatory 
		if len(operands)!=2:
			raise InvalidInstException()		

		# Only reg|reg, reg|imm08 and reg|imm16 are supported
		if operands[0].getType()!=Operand.OP_IND_REG16 or operands[1].getType()!=Operand.OP_IND_REG16:
			raise InvalidInstException() 
					 
	def getSize(self):
		return 2 + self.getOpSize()
						
	def toBytes(self):
		return "%c\x09"%self.opcode+self.operands[0].toBytes()+self.operands[1].toBytes()

"""
ADD, ADDB, ADDL
"""

class AddInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_ADD, operands)

		
class AddBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_ADD, operands)


class AddLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_ADD, operands)

"""
SUB, SUBB, SUBL
"""

class SubInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_SUB, operands)

		
class SubBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_SUB, operands)


class SubLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_SUB, operands)


"""
DIV, DIVB, DIVL
"""

class DivInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_DIV, operands)

		
class DivBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_DIV, operands)


class DivLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_DIV, operands)

"""
MUL, MULB, MULL
"""

class MulInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_MUL, operands)

		
class MulBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_MUL, operands)


class MulLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_MUL, operands)

"""
OR, ORB, ORL
"""

class OrInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_OR, operands)

		
class OrBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_OR, operands)


class OrLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_OR, operands)

"""
AND, ANDB, ANDL
"""

class AndInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_AND, operands)

		
class AndBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_AND, operands)


class AndLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_AND, operands)
		
"""
XOR, XORB, XORL
"""

class XorInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_XOR, operands)

		
class XorBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_XOR, operands)


class XorLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_XOR, operands)


"""
PUSH, PUSHB, PUSHL
"""

class PushInst(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_PUSH, operands, flag=True)

		
class PushBInst(Imm8Inst):
	def __init__(self, line, operands):
		Imm8Inst.__init__(self, line, OP_PUSH, operands, flag=True)


class PushLInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_PUSH, operands, flag=True)


"""
INC, DEC,NOT
"""

class IncInst(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_INC, operands)
		
class DecInst(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_DEC, operands)

class NotInst(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_NOT, operands)

"""
POP
"""

class PopInst(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_POP, operands)
		
"""
JZ,JNZ,JA,JB
"""

class JzInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JZ, operands)
	
	def toBytes(self):
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += _op.toBytes()
		return s
		
		
class JnzInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JNZ, operands)
		
	def toBytes(self):
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += _op.toBytes()
		return s

class JaInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JA, operands)
	
	def toBytes(self):
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += _op.toBytes()
		return s
		
		
class JbInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JB, operands)
		
	def toBytes(self):
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += _op.toBytes()
		return s



"""
JMPL,JMPS
"""

class JmplInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JMPL, operands)

	def toBytes(self):
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		#print 'delta: %4x' % delta
		#print 'size: %d' % self.getSize()
		#print 'orig: %4x' % orig
		#print 'dest: %4x' % dest
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		s += _op.toBytes()
		return s
		
class JmpsInst(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_JMPS, operands)

	def getSize(self):
			return 2

	def toBytes(self):

		if abs(self.operands[0].getValue() - self.offset + self.getSize())>127:
			print abs(self.operands[0].getValue() - self.offset + self.getSize())
			raise InvalidInstException()	
		
		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFF		
		_op = Operand('#x%04x'%delta)
		_op.toImm08()
		s = '%c'%self.opcode
		s += _op.toBytes()
		return s

		

"""
END,NOP,RET
"""
class EndInst(NakedInst):
	def __init__(self, line, operands):
		NakedInst.__init__(self, line, OP_END, operands)

class NopInst(NakedInst):
	def __init__(self, line, operands):
		NakedInst.__init__(self, line, OP_NOP, operands)

class RetInst(NakedInst):
	def __init__(self, line, operands):
		NakedInst.__init__(self, line, OP_RET, operands)

"""
TEST,XCHG
"""

class TestInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_TEST, operands, flag=False)
		
class XchgInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_XCHG, operands, flag=False)
		
"""
CMP,CMPB,CMPL
"""

class CmpInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_CMP, operands)

		
class CmpBInst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_CMP, operands)


class CmpLInst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_CMP, operands)

"""
CALL
"""

class CallImm16(Imm16Inst):
	def __init__(self, line, operands):
		Imm16Inst.__init__(self, line, OP_CALL, operands, flag=True)


	def toBytes(self):

		dest = self.operands[0].getValue()
		orig =self.offset+self.getSize()
		delta = (self.operands[0].getValue() - (self.offset+self.getSize()))&0xFFFF
		_op = Operand('#x%04x'%delta)
		_op.toImm16()
		s = '%c'%self.opcode
		if self.flag:
			s += '\x04'
		s += _op.toBytes()
		return s
		
	
class CallReg16(RegInst):
	def __init__(self, line, operands):
		RegInst.__init__(self, line, OP_CALL, operands, flag=True)
		
class CallInst:
	def __init__(self, line, operands):
		if len(operands)!=1:
			raise InvalidInstException()
		if operands[0].getType()==Operand.OP_IMM16:
			self.__inst = CallImm16(line, operands)
		elif operands[0].getType()==Operand.OP_REG16:
			self.__inst = CallReg16(line, operands)
		else:
			raise InvalidInstException()

	def updateReference(self, ref, value):
		return self.__inst.updateReference(ref,value)

	def hasUnresolvedRefs(self):
		return self.__inst.hasUnresolvedRefs()	

	def setOffset(self, offset):
		return self.__inst.setOffset(offset)
		
	def getOffset(self):
		return self.__inst.getOffset()

	def getSize(self):
		return self.__inst.getSize()
		
	def toBytes(self):
		return self.__inst.toBytes()
		
"""
MOV,MOVB,MOVL
"""
	
class MovRegRegInst(RegRegInst):
	def __init__(self, line, operands):
		RegRegInst.__init__(self, line, OP_MOV, operands)	
	
class MovRegImm8Inst(RegImm8Inst):
	def __init__(self, line, operands):
		RegImm8Inst.__init__(self, line, OP_MOV, operands)
		
class MovRegImm16Inst(RegImm16Inst):
	def __init__(self, line, operands):
		RegImm16Inst.__init__(self, line, OP_MOV, operands)

class MovRegIndInst(RegIndInst):
	def __init__(self, line, operands):
		RegIndInst.__init__(self, line, OP_MOV, operands)
		
class MovIndRegInst(IndRegInst):
	def __init__(self, line, operands):
		IndRegInst.__init__(self, line, OP_MOV, operands)		
		
class MovIndImm8Inst(IndImm8Inst):
	def __init__(self, line, operands):
		IndImm8Inst.__init__(self, line, OP_MOV, operands)
	
class MovIndImm16Inst(IndImm16Inst):
	def __init__(self, line, operands):
		IndImm16Inst.__init__(self, line, OP_MOV, operands)
		
class MovIndIndInst(IndIndInst):
	def __init__(self, line, operands):
		IndIndInst.__init__(self, line, OP_MOV, operands)

class MovInst:
	def __init__(self, line, operands):
		if len(operands)!=2:
			raise InvalidInstException()
		t1 = operands[0].getType()
		t2 = operands[1].getType()
		if t1==Operand.OP_REG16:
			if t2==Operand.OP_REG16:
				self.__inst = MovRegRegInst(line, operands)
			elif t2==Operand.OP_IND_REG16:
				self.__inst = MovRegIndInst(line, operands)
			else:
				raise InvalidInstException()
		elif t1==Operand.OP_IND_REG16:
			if t2==Operand.OP_REG16:
				self.__inst = MovIndRegInst(line, operands)
			elif t2==Operand.OP_IND_REG16:
				self.__inst = MovIndIndInst(line,operands)
			else:
				raise InvalidInstException()
		else:
			raise InvalidInstException()

	def updateReference(self, ref, value):
		return self.__inst.updateReference(ref,value)

	def hasUnresolvedRefs(self):
		return self.__inst.hasUnresolvedRefs()	
	
	def setOffset(self, offset):
		return self.__inst.setOffset(offset)
		
	def getOffset(self):
		return self.__inst.getOffset()
					
	def getSize(self):
		return self.__inst.getSize()
		
	def toBytes(self):
		return self.__inst.toBytes()

class MovBInst:
	def __init__(self, line, operands):
		if len(operands)!=2:
			raise InvalidInstException()
		t1 = operands[0].getType()
		t2 = operands[1].getType()
		if t1==Operand.OP_REG16:
			if t2==Operand.OP_IMM08:
				self.__inst = MovRegImm8Inst(line, operands)
			else:
				raise InvalidInstException()
		elif t1==Operand.OP_IND_REG16:
			if t2==Operand.OP_IMM08:
				self.__inst = MovIndImm8Inst(line, operands)
			else:
				raise InvalidInstException()
		else:
			raise InvalidInstException()

	def updateReference(self, ref, value):
		return self.__inst.updateReference(ref,value)

	def hasUnresolvedRefs(self):
		return self.__inst.hasUnresolvedRefs()	

	def setOffset(self, offset):
		return self.__inst.setOffset(offset)
		
	def getOffset(self):
		return self.__inst.getOffset()

	def getSize(self):
		return self.__inst.getSize()
		
	def toBytes(self):
		return self.__inst.toBytes()	
			
class MovLInst:
	def __init__(self, line, operands):
		if len(operands)!=2:
			raise InvalidInstException()
		t1 = operands[0].getType()
		t2 = operands[1].getType()
		if t1==Operand.OP_REG16:
			if t2==Operand.OP_IMM16 or t2==Operand.OP_IMM08:
				self.__inst = MovRegImm16Inst(line, operands)
			elif t2==Operand.OP_IND_REG16:
				self.__inst = MovRegIndInst(line, operands)
			else:
				raise InvalidInstException()
		elif t1==Operand.OP_IND_REG16:
			if t2==Operand.OP_IMM08 or t2==Operand.OP_IMM16:
				self.__inst = MovIndImm16Inst(line, operands)
			else:
				raise InvalidInstException()
		else:
			raise InvalidInstException()

	def setOffset(self, offset):
		return self.__inst.setOffset(offset)
		
	def getOffset(self):
		return self.__inst.getOffset()
		
	def updateReference(self, ref, value):
		return self.__inst.updateReference(ref,value)

	def hasUnresolvedRefs(self):
		return self.__inst.hasUnresolvedRefs()		

	def getSize(self):
		return self.__inst.getSize()
		
	def toBytes(self):
		return self.__inst.toBytes()
		
	

"""
Syscall
"""

class SyscallInst(NakedInst):
	def __init__(self, line, operands):
		NakedInst.__init__(self, line, OP_SYSCALL, operands)


def dispatchInst(line, operator, operands=[]):
	operators = {
		'add':AddInst,
		'addb':AddBInst,
		'addl':AddLInst,
		'sub':SubInst,
		'subb':SubBInst,
		'subl':SubLInst,
		'mul':MulInst,
		'mulb':MulBInst,
		'mull':MulLInst,
		'div':DivInst,
		'divb':DivBInst,
		'divl':DivLInst,
		'or':OrInst,
		'orb':OrBInst,
		'orl':OrLInst,
		'xor':XorInst,
		'xorb':XorBInst,
		'xorl':XorLInst,
		'and':AndInst,
		'andb':AndBInst,
		'andl':AndLInst,
		'not':NotInst,
		'push':PushInst,
		'pushb':PushBInst,
		'pushl':PushLInst,
		'inc':IncInst,
		'dec':DecInst,
		'nop':NopInst,
		'end':EndInst,
		'ret':RetInst,
		'mov':MovInst,
		'movb':MovBInst,
		'movl':MovLInst,
		'jz':JzInst,
		'jnz':JnzInst,
		'jmps':JmpsInst,
		'jmpl':JmplInst,
		'cmp':CmpInst,
		'cmpb':CmpBInst,
		'cmpl':CmpLInst,
		'test':TestInst,
		'pop':PopInst,
		'xchg':XchgInst,
		'syscall':SyscallInst,
		'call':CallInst,
		'ja':JaInst,
		'jb':JbInst,
	}
	if operator in operators:
		return operators[operator](line, operands)
