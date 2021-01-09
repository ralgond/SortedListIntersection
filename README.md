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
v1_avx2	189ms	52910.1	6
v3_avx2	414ms	24154.6	6
highlyscalable_SIMD	155ms	64516.1	6
highlyscalable_avx2	160ms	62500	6
lemire_highlyscalable_SIMD	160ms	62500	6
scalar	152ms	65789.5	6
block_merge	571ms	17513.1	6
sttni	168ms	59523.8	6
bisearch	4002ms	2498.75	6
bloom	3931ms	2543.88	6
```
可以看出scalar算法(最简单的算法)在众算法中最快，而bloom过滤器慢得出乎意料。

### 数据集2（数量为10w*10w，交集数为8944）
./main data/10w_1.txt data/10w_2.txt 
```
v1_avx2	1884ms	5307.86	8944
v3_avx2	3472ms	2880.18	8944
highlyscalable_SIMD	1942ms	5149.33	8944
highlyscalable_avx2	1081ms	9250.69	8944
lemire_highlyscalable_SIMD	1751ms	5711.02	8944
scalar	7591ms	1317.35	8944
block_merge	4821ms	2074.26	8944
sttni	2198ms	4549.59	8944
```
可以看出highlyscalable_avx2最快，它是[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法的升级版。

### 数据集3（数量为10w*10w，交集数为956）
./main data/10w_3.txt data/10w_4.txt
```
v1_avx2	1951ms	5125.58	956
v3_avx2	2829ms	3534.82	956
highlyscalable_SIMD	2086ms	4793.86	956
highlyscalable_avx2	1185ms	8438.82	956
lemire_highlyscalable_SIMD	1867ms	5356.19	956
scalar	7523ms	1329.26	956
block_merge	4859ms	2058.04	956
sttni	1355ms	7380.07	956
```
结合数据集2的结果可以看出，除了基于sttni的算法，交集的大小对其他算法的性能影响很小。而对于基于sttni的算法，交集越小速度越快。

### 数据集4（数量为100w*100w，交集数为9792）
```
v1_avx2	19932ms	501.706	9792
v3_avx2	28788ms	347.367	9792
highlyscalable_SIMD	21565ms	463.714	9792
highlyscalable_avx2	12725ms	785.855	9792
lemire_highlyscalable_SIMD	19159ms	521.948	9792
scalar	75863ms	131.817	9792
block_merge	50742ms	197.075	9792
sttni	15344ms	651.721	9792
```
当数量级达到100w时，highlyscalable_avx2算法速度依然是最快的。

### 结论
推荐使用highlyscalable_avx2算法，它简单、不依赖额外内存空间且快速最快。
