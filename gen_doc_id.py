import sys
import random
import time

random.seed(int(time.time()))

cnt = int(sys.argv[1])

for i in range(0, cnt):
    num = random.randint(0, 100000000)
    print(num)
