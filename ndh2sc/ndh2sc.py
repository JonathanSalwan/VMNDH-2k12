#!/usr/bin/python

import sys
from random import choice

def disp(r):
	s = '\\x'+'\\x'.join(['%02x'%ord(c) for c in r])
	print 'shellcode="%s"'%s
	print 'shellcode len: %d bytes' % len(r)

def w2i(w):
	return (ord(w[0])|(ord(w[1])<<8))

def ndh2bin(filename):
	bin = open(filename,'rb').read()
	size = w2i(bin[4:6])
	code = bin[6:6+size]
	return (size,code)

def findxorkey(code):
	keys = []
	for i in range(1,255):
		if chr(i) not in code:
			keys.append(i)
	if len(keys)>0:
		return choice(keys)
	else:
		return None	

def poly(size,code):
	k = findxorkey(code)
	if k is not None:
		enc_code = ''.join([chr(ord(c)^k) for c in code])
		stub="\x16\x1e\x03\x01\x04\x01\x02%c\x04\x0a\x03\x01\x0e\x01\x03%c\x04\x06\x01\x03\x0a\x01\x0b\x02\x17\x02\x02\x11\xea\xff\x16\x04\x19\x04\xde\xff" % (size,k)
		return stub + enc_code
	else:
		return None

if __name__ == '__main__':
	if len(sys.argv)==2:
		size,code = ndh2bin(sys.argv[1])
		res = poly(size,code)
		if res:
			disp(res)
		else:
			print '[!] Unable to poly !'
	else:
		print 'NDH 2 Shellcode - (c) virtualabs'
		print 'convert NDH compiled code to shellcode'
		print ''
		print 'Usage: %s [ndh shellcode file]'

