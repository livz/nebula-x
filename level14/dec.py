# Level 14 'decryptor'
# python dec.py `cat /home/flag14/token`

import sys

input = sys.argv[1]

out = [0 for i in range(0, len(input))]

key = 0
for i in range(0, len(input)):
	out[i] = chr(ord(input[i]) - key)
	key += 1
	
print "".join(out)