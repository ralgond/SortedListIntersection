import sys
import random
import time

random.seed(int(time.time()))

cnt = int(sys.argv[1])
max_value = int(sys.argv[2])

for i in range(0, cnt):
    num = random.randint(0, max_value)
    print(num)
