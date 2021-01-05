# SortedListIntersection
有序列表求交集算法

# 如何运行
make

./main data/docid_list_s.txt data/docid_list_c.txt

# 性能
### 数据集1（数量为2w*2w，交集数为6）
./main data/docid_list_s.txt data/docid_list_c.txt

结果有3列，第一列是算法名，第二列是花费时间，第三列是QPS，第四列是交集大小。比如下面：
```
v1_avx2	189ms	52910.1	6
v3_avx2	360ms	27777.8	6
highlyscalable_SIMD	156ms	64102.6	6
lemire_highlyscalable_SIMD	161ms	62111.8	6
scalar	122ms	81967.2	6
block_merge	550ms	18181.8	6
bisearch	4002ms	2498.75	6
bloom	3931ms	2543.88	6
```
可以看出scalar算法(最简单的算法)在众算法中最快，而bloom过滤器慢得出乎意料。

### 数据集2（数量为10w*10w，交集数为8944）
./main data/10w_1.txt data/10w_2.txt 
```
v1_avx2	1876ms	5330.49	8944
v3_avx2	3119ms	3206.16	8944
highlyscalable_SIMD	1880ms	5319.15	8944
lemire_highlyscalable_SIMD	1814ms	5512.68	8944
scalar	6806ms	1469.29	8944
block_merge	4521ms	2211.9	8944
```
可以看出[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法在众算法中算是比较快的，不过它相对v1_avx2更复杂。

### 数据集3（数量为10w*10w，交集数为956）
./main data/10w_3.txt data/10w_4.txt
```
v1_avx2	1959ms	5104.65	956
v3_avx2	2465ms	4056.8	956
highlyscalable_SIMD	2016ms	4960.32	956
lemire_highlyscalable_SIMD	1941ms	5151.98	956
scalar	6728ms	1486.33	956
block_merge	4580ms	2183.41	956
```
结合数据集2的结果可以看出，交集的大小对算法的性能影响很小。

### 数据集4（数量为100w*100w，交集数为10096）
```
v1_avx2	21127ms	473.328	10096
v3_avx2	25731ms	388.636	10096
highlyscalable_SIMD	21281ms	469.903	10096
lemire_highlyscalable_SIMD	20229ms	494.34	10096
scalar	67708ms	147.693	10096
block_merge	46231ms	216.305	10096
```
