import re
import os
from preproc import NdhPreproc,InvalidDirective
from core.operand import InvalidOperandException,Operand
from core.instset import dispatchInst,InvalidInstException
from core.meta import Label,Data,InvalidMetaException

class UnresolvedReferences(Exception):
	def __init__(self, filename, unresolved_refs):
		Exception.__init__(self)
		self.filename = filename
		self.refs = unresolved_refs
	def __str__(self):
		r = '\n'.join(['(%s) Unresolved reference %s' %(self.filename,ref[1:]) for ref in self.refs]) 
		return r

class NdhSyntaxError(Exception):
	def __init__(self, filename='main', msg='', line=None):
		Exception.__init__(self)
		self.filename = filename
		self.line = line+1
		self.msg = msg
		
	def __str__(self):
		s = '[%s'%self.filename
		if self.line:
			s += ':%d] '%self.line
		else:
			s += '] '
		s += self.msg
		return s
				
		
		return '(%s) %s line #%d' % (self.filename,self.msg,self.line_nb)
	def __repr__(self):
		return '<NdhSyntaxError line num=%d>' % self.line_nb
		

class NdhCompiler:
	
	def __init__(self):
		self.filename = '~'
		return

	def __is_meta(self, line):
		return False

	def is_meta(self, line):
		return line.strip()[0]=='.'

	def __parse_label(self, value):
		if re.match('^[@a-z_]+[0-9]*$', value):
			return value
		else:
			return None
	
	def parse_meta(self, filename, line, raw):
		p = re.compile('^\s*\.(label|db)\s*(.*)(;.*)?',re.I)
		r = p.match(raw)
		if r:
			meta_type = r.group(1)
			meta_value = r.group(2)
			if meta_type=='label':
				name = self.__parse_label(meta_value)
				return Label(name)
			else:
				data = Data(meta_value)
				return data
		raise NdhSyntaxError(filename=filename,line=line+1, msg='Unknown meta-data')
	
	def parse_inst(self, filename, line, raw):
		# extract operation and operands
		# format: operation operand1, operand2
		p = re.compile('^\s*(\S+)(\s*([@a-z0-9_#:\[\]\s]+),?)?(\s*([@a-z0-9_#:\[\]]+),?)?\s*(;.*)?$',re.I)
		r = p.match(raw)
		if r:
			try:
				op1 = r.group(3)
				op2 = r.group(5)
				if op1 is not None:
					if op2 is not None:
						operands = [Operand(op1.strip()),Operand(op2.strip())]
					else:
						operands = [Operand(op1.strip())]
				else:
					operands = []
				operator = r.group(1).lower()
				inst = dispatchInst(line, operator, operands)
				return inst
			except InvalidOperandException,e:
				raise NdhSyntaxError(filename=filename,line=line,msg='Invalid operand: %s' % raw)
			except InvalidInstException,e:
				raise NdhSyntaxError(filename=filename,line=line,msg='Invalid instruction: %s' % raw)
		else:
			print raw
			raise NdhSyntaxError(filename=filename,line=line+1,msg='Syntax error: %s'%raw)

	def parse_file(self, filename):
		self.filename = os.path.basename(filename)
		return self.parse_content(open(filename,'r').read(), bootloader=True,filename=filename)
			
	def parse_content(self, content, bootloader=False,filename='~'):
		ndh_preproc = NdhPreproc(content)
		lines = ndh_preproc.process_content(filename=filename)

		#print '\n'.join([l[2] for l in lines])
		
		# insert bootloader if required
		if bootloader:		
			lines = [('~',0,".label __entrypoint"),('~',0,'jmpl :main')]+lines
		out = []
		offset = 0x8000
		for filename,line,raw in lines:
			if len(raw.strip())>0:
				raw = raw.strip()
				if raw[0]!=';':
					if self.is_meta(raw):
						_meta = self.parse_meta(filename,line, raw)
						_meta.setOffset(offset)
						#if isinstance(_meta, Label):
						#	print '%s: %04X' % (_meta.getName(), _meta.getOffset())
						if isinstance(_meta,Data):
							offset += _meta.getSize()
						out.append(_meta)
					else:
						_inst = self.parse_inst(filename,line, raw)
						if _inst is None:
							raise NdhSyntaxError(filename=filename,line=line,msg='Invalid instruction: %s'%raw)
						_inst.setOffset(offset)
						offset += _inst.getSize()
						out.append(_inst)
		return out
	
	def check_syntax(self, filename):
		try:
			self.filename = os.path.basename(filename)
			# remove preproc directives before checking syntax
			content = open(filename,'r').read()
			preproc = NdhPreproc(content)
			_content = preproc.process_content()

			#print _content
			#_content = preproc.remove_directives()
		
			# check main file syntax
			insts = self.parse_content(_content, preproc=False)
		
			# loop on every included file
			preproc = NdhPreproc(content)
			included_files = preproc.get_includes()
			for f in included_files:
				self.check_syntax(f)
			
		except InvalidDirective,e:
			raise NdhSyntaxError(filename=self.filename,line=e.line,msg='Invalid directive: %s'%e.directive)
	
	def assemble_link(self, insts):

		# solve self-refs and unnamed labels
		for i in range(len(insts)):
			inst = insts[i]
			if not isinstance(inst, Label) and not isinstance(inst, Data):
				inst.updateReference(':$','#x%04X'%inst.getOffset())
			elif isinstance(inst, Label):
				if inst.name=='@@' and i>0:
					# loop backward until another '@@' label is found and update '@f'					
					for j in range(i-1, -1, -1):
						_inst = insts[j]
						if isinstance(_inst, Label):
							if _inst.name == '@@':
								break
						elif not isinstance(_inst, Data):
							_inst.updateReference(':@f','#x%04X'%inst.getOffset())
				if inst.name=='@@' and i<(len(insts)-1):
					# loop forward until another '@@' label is found and update '@b'
					for j in range(i+1,len(insts)):
						_inst = insts[j]
						if isinstance(_inst, Label):
							if _inst.name == '@@':
								break
						elif not isinstance(_inst, Data):
							_inst.updateReference(':@b', '#x%04X'%inst.getOffset())
		
		# solve links
		for inst in insts:
			if isinstance(inst, Label):
				for _inst in insts:
					if not isinstance(_inst, Label) and not isinstance(_inst,Data):
						_inst.updateReference(':%s'%inst.name,'#x%04X'%inst.getOffset())
						
		# check if no more ref exist
		left_ref = 0
		unresolved_refs = []
		for inst in insts:
			if not isinstance(inst, Label) and not isinstance(inst, Data):
				if inst.hasUnresolvedRefs():
					unresolved_refs.append(inst.hasUnresolvedRefs())
					left_ref += 1
				
		if left_ref==0:
			# produce output bytes
			output = []
			for inst in insts:
				if not isinstance(inst,Label):
					#print inst
					#print ''.join(['%02X '%ord(c) for c in inst.toBytes()])
					for b in inst.toBytes():
						output.append(b)
			return output
		else:
			raise  UnresolvedReferences(self.filename, unresolved_refs)
		
if __name__ == '__main__':
	d = NdhCompiler()
	print 'parsing ...'
	r = d.parse_file('test.asm')
	print 'linking ...'
	output = ''.join(d.link(r))
	s = len(output)
	size = [0,0]
	for i in range(2):
		size[i] = s&0xFF
		s = s>>8
	_size = ''.join([chr(z) for z in size])
	open('test.ndh','wb').write('.NDH'+_size+output)