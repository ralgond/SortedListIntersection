import sys
d = {}
for line in open(sys.argv[1]):
    w = line.strip()
    d[w] = 1

for line in open(sys.argv[2]):
    w = line.strip()
    if d.get(w) != None:
        print(w)
