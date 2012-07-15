name="/home/flag18/password"
 
for i in range(0, len(name)/4) :
  print "0x"+"".join([hex(ord(name[i*4+j])).replace('0x','') for j in range (3,-1,-1)])
  
# last letter 'd' not converted  
print hex(ord('d'))