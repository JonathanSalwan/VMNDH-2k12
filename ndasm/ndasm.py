#!/usr/bin/python

"""
NDH Virtual Machine compiler - Virtualabs
"""

import sys
from asm.preproc import NdhPreproc,InvalidDirective
from asm.parser import NdhCompiler,NdhSyntaxError,UnresolvedReferences
from optparse import OptionParser

def build_parser():
	usage = 'usage: %prog [options]'
	parser = OptionParser(usage)
	parser.add_option('-i','--input',dest='input',help='ASM input file')
	parser.add_option('-o','--output',default='out.ndh',dest='output',help='output file')
	return parser

def main():
	parser = build_parser()
	(options,args) = parser.parse_args()
	if options.input:
		ndh_comp = NdhCompiler()
		print '[*] Parsing source file ...'
		try:
			# read input file
			content = open(options.input,'r').read()
			# insert bootloader
			print '[+] Assembling ...'
			insts = ndh_comp.parse_file(options.input)
			print '[*] Linking ...'
			bytes = ''.join(ndh_comp.assemble_link(insts))
			print '[*] Creating outfile ...'		
			s = len(bytes)
			size = [0,0]
			for i in range(2):
				size[i] = s&0xFF
				s = s>>8
			_size = ''.join([chr(z) for z in size])
			open(options.output,'wb').write('.NDH'+_size+bytes)
			print '[*] Done. %d bytes written.' % len(bytes)
		except IOError,e:
			print '[!] File R/W error'
		except NdhSyntaxError,e:
			print '[!] %s' % str(e)
		except InvalidDirective,e:
			print '[!] Preprocessor error - %s' % str(e)
		except UnresolvedReferences,e:
			print '[!] %s' % str(e)
	else:
		print '[!] Input file is missing !'

if __name__ == '__main__':
	main()