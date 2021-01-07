# SortedListIntersection
有序列表求交集算法

# 如何运行
make

./main data/docid_list_s.txt data/docid_list_c.txt

# 性能
### 数据集1（数量为2w*2w，交集数为6）
./main data/docid_list_s.txt data/docid_list_c.txt

结果有4列，第一列是算法名，第二列是花费时间，第三列是QPS，第四列是交集大小。比如下面：
```
v1_avx2	198ms	50505.1	6
v3_avx2	415ms	24096.4	6
highlyscalable_SIMD	154ms	64935.1	6
lemire_highlyscalable_SIMD	182ms	54945.1	6
scalar	149ms	67114.1	6
block_merge	636ms	15723.3	6
sttni	167ms	59880.2	6
bisearch	4002ms	2498.75	6
bloom	3931ms	2543.88	6
```
可以看出scalar算法(最简单的算法)在众算法中最快，而bloom过滤器慢得出乎意料。

### 数据集2（数量为10w*10w，交集数为8944）
./main data/10w_1.txt data/10w_2.txt 
```
v1_avx2	1891ms	5288.21	8944
v3_avx2	3528ms	2834.47	8944
highlyscalable_SIMD	1784ms	5605.38	8944
lemire_highlyscalable_SIMD	1908ms	5241.09	8944
scalar	7725ms	1294.5	8944
block_merge	5111ms	1956.56	8944
sttni	2309ms	4330.88	8944
```
可以看出[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法在众算法中算是比较快的，不过它相对v1_avx2更复杂。

### 数据集3（数量为10w*10w，交集数为956）
./main data/10w_3.txt data/10w_4.txt
```
v1_avx2	1951ms	5125.58	956
v3_avx2	2895ms	3454.23	956
highlyscalable_SIMD	1944ms	5144.03	956
lemire_highlyscalable_SIMD	2083ms	4800.77	956
scalar	7497ms	1333.87	956
block_merge	5151ms	1941.37	956
sttni	1416ms	7062.15	956
```
结合数据集2的结果可以看出，除了基于sttni的算法，交集的大小对其他算法的性能影响很小。而对于基于sttni的算法，交集越小速度越快。

### 数据集4（数量为100w*100w，交集数为9792）
```
v1_avx2	20067ms	498.331	9792
v3_avx2	29189ms	342.595	9792
highlyscalable_SIMD	19818ms	504.592	9792
lemire_highlyscalable_SIMD	21090ms	474.158	9792
scalar	75904ms	131.745	9792
block_merge	52031ms	192.193	9792
sttni	15090ms	662.691	9792
```
当数量级达到100w时，基于sttni加速的算法速度最快。

### 结论
基于STTNI的算法虽然不是最快的，但是它在各种情况中的速度都名列前茅，不过STTNI是需要SSE4.2，导致它的可移植性比较差，另外这个算法还需要额外的内存空间。

v1_avx2是最简单，无需额外内存空间，在各种情况也是名列前茅。我比较推荐使用这个算法。
