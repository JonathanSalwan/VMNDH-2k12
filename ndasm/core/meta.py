import re

class InvalidMetaException(Exception):
	def __init__(self):
		Exception.__init__(self)

class Label:
	def __init__(self, name=None, offset=None):
		self.name = name
		self.offset = offset

	def setOffset(self, offset):
		self.offset = offset
		
	def getOffset(self):
		return self.offset
		
	def getName(self):
		return self.name
		
class Data:
	def __init__(self, value, offset=None):
		self.value = value
		self.offset = offset
		# parse value
		self.explode()

	def setOffset(self, offset):
		self.offset = offset
		
	def getOffset(self):
		return self.offset
		
	def getSize(self):
		return len(self.bytes)
	
	def explode(self):
		bytes = []
		items = self.value.split(',')
		for item in items:
			item = item.strip()
			if re.match('"([^"]*)"',item):
				s = re.match('"([^"]*)"',item).group(1)
				for c in s:
					bytes.append(ord(c))
			elif re.match('^[0-9]+$', item):
				v = re.match('([0-9]+)', item).group(1)
				try:
					v = int(v)
					if v>=0 and v<=255:
						bytes.append(v)
					else:
						raise InvalidMetaException()
				except ValueError,e:
					raise InvalidMetaException()
			elif re.match('^(0x[0-9a-f]+)$',item, re.I):
				v = re.match('^(0x[0-9a-f]+)$',item,re.I).group(1)
				try:
					v = int(v, 16)
					if v>=0 and v<=255:
						bytes.append(v)
					else:
						raise InvalidMetaException()
				except ValueError,e:
					raise InvalidMetaException()
		self.bytes = ''.join([chr(c) for c in bytes])
		
	def toBytes(self):
		return self.bytes