"""
VMNDH Preprocessor

Defined directives:
	
#define STR REPLACEMENT
#include FILENAME

"""

import re,os
import here

class InvalidDirective(Exception):
	def __init__(self, line):
		Exception.__init__(self)
		self.line = line
	def __str__(self):
		return '(%s:%d) Invalid directive: %s' % (self.line[0],self.line[1]+1,self.line[2])

class NdhPreproc:
	
	DIRECTIVES = ['include','define']	
	
	def __init__(self, content):
		self.content = content

	def get_includes(self, content=None):
		# extract directives
		if content is None:
			lines = self.content.split('\n')
		else:
			lines = content.split('\n')
		
		# extract included files
		included_files = [] 		
		directives = self.extract_directives(lines)
		for l,d,a1,a2 in directives:
			if d=='include':
				included_files.append(a1)
		return included_files

	def extract_directives(self, lines):
		directives = []
		for i in range(len(lines)):
			r = self.is_directive(lines[i])
			if r:
				directive,arg1,arg2 = r
				directives.append((i,directive,arg1,arg2))
		return directives

	def remove_directives(self,content=None):
		out = []
		# extract directives
		if content is None:
			lines = self.content.split('\n')
		else:
			lines = content.split('\n')
		try:
			for i in range(len(lines)):
				if not self.is_directive(lines[i]):
					out.append(lines[i])
			return '\n'.join(out)	
		except InvalidDirective,e:
			raise InvalidDirective(lines[i])

	def extract_defines(self, lines):
		directives = []
		for i in range(len(lines)):
			r = self.is_directive(lines[i])
			if r:
				directive,arg1,arg2 = r
				if directive=='define':				
					directives.append((i,directive,arg1,arg2))
		return directives

	def remove_defines(self, lines):
		out_lines = []
		for i in range(len(lines)):
			r = self.is_directive(lines[i])
			if r:
				directive,arg1,arg2 = r
				if directive!='define':				
					out_lines.append(lines[i])
			else:
				out_lines.append(lines[i])
		return out_lines
		

	def is_directive(self, line):
		r = re.match('^\s*#([a-z]+)(\s*([a-z0-9\._\/]+))?(\s*"([^"]+)")?\s*$',line[2],re.I)
		if r:		
			directive = r.group(1).lower()
			arg1 = r.group(3)
			arg2 = r.group(5)
			if directive not in NdhPreproc.DIRECTIVES:
				raise InvalidDirective(line)			
			
			return (directive,arg1,arg2) 
		else:
			return None

	def process_define(self, pattern, replacement, lines):
		out_lines = []
		for i in range(len(lines)):
			out_lines.append((lines[i][0],lines[i][1],lines[i][2].replace(pattern,replacement)))
		return out_lines

	def process_includes(self, filename, lines):
		out_lines = []
		for line in lines:
			if self.is_directive(line):
				directive, arg1,arg2 = self.is_directive(line)
				if directive == 'include':
					_lines = self.process_content(open(os.path.join(os.path.dirname(here.__file__),arg1),'r').read(), include_only=True,filename=filename)
					for _l in _lines:
						out_lines.append(_l)
				else:
					out_lines.append(line)
			else:
				out_lines.append(line)
		return out_lines

	def process_directives(self, content=None,include_only=False,filename='~'):
		# extract directives
		if content is None:
			lines = self.content.split('\n')
		else:
			lines = content.split('\n')
		_lines = [(filename,i,lines[i]) for i in range(len(lines))]
		lines = _lines
			
		lines = self.process_includes(filename, lines)
			
		if not include_only:
			defines = self.extract_defines(lines)
			_lines = self.remove_defines(lines)
			for l,d,a1,a2 in defines:
				_lines = self.process_define(a1,a2,_lines)
			lines = _lines

		return lines
		
	def process_content(self,content=None, include_only=False, filename='~'):
		lines = self.process_directives(content, include_only=include_only, filename=filename)
		return lines
					
		
		
if __name__ == '__main__':
	content=open('test_preproc.asm','r').read()
	p = NdhPreproc(content)
	print p.process_content()