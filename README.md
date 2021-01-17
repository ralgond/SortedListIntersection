# SortedListIntersection
有序列表求交集算法

# 如何运行
首先要确保你的CPU在指令集方面支持AVX2, AVX512F和SSE4.2的STTNI

make

./main data/docid_list_s.txt data/docid_list_c.txt

# 性能
### CPU配置
Intel(R) Xeon(R) Gold 6146 CPU @ 3.20GHz

### 数据集1（数量为2w*2w，交集数为6）
./main data/docid_list_s.txt data/docid_list_c.txt

结果有4列，第一列是算法名，第二列是花费时间，第三列是QPS，第四列是交集大小。比如下面：
```
v1_avx2 185ms   54054.1 6
v3_avx2 365ms   27397.3 6
highlyscalable_SIMD     157ms   63694.3 6
highlyscalable_avx2     161ms   62111.8 6
highlyscalable_avx512   243ms   41152.3 6
lemire_highlyscalable_SIMD      179ms   55865.9 6
scalar  136ms   73529.4 6
block_merge     488ms   20491.8 6
sttni   199ms   50251.3 6
bisearch	4002ms	2498.75	6
bloom	3931ms	2543.88	6
```
可以看出scalar算法(最简单的算法)在众算法中最快，而bloom过滤器慢得出乎意料。

### 数据集2（数量为10w*10w，交集数为8944）
./main data/10w_1.txt data/10w_2.txt 
```
v1_avx2 1889ms  5293.81 8944
v3_avx2 3122ms  3203.07 8944
highlyscalable_SIMD     2053ms  4870.92 8944
highlyscalable_avx2     1169ms  8554.32 8944
highlyscalable_avx512   1413ms  7077.14 8944
lemire_highlyscalable_SIMD      2188ms  4570.38 8944
scalar  6890ms  1451.38 8944
block_merge     4341ms  2303.62 8944
sttni   2298ms  4351.61 8944
```
可以看出highlyscalable_avx2最快，它是[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法的升级版。

### 数据集3（数量为10w*10w，交集数为956）
./main data/10w_3.txt data/10w_4.txt
```
v1_avx2 1942ms  5149.33 956
v3_avx2 2470ms  4048.58 956
highlyscalable_SIMD     2182ms  4582.95 956
highlyscalable_avx2     1278ms  7824.73 956
highlyscalable_avx512   1500ms  6666.67 956
lemire_highlyscalable_SIMD      2325ms  4301.08 956
scalar  6792ms  1472.32 956
block_merge     4299ms  2326.12 956
sttni   1543ms  6480.88 956
```
结合数据集2的结果可以看出，除了基于sttni的算法，交集的大小对其他算法的性能影响很小。而对于基于sttni的算法，交集越小速度越快。

### 数据集4（数量为100w*100w，交集数为9792）
```
v1_avx2 20172ms 495.737 9792
v3_avx2 24989ms 400.176 9792
highlyscalable_SIMD     22872ms 437.216 9792
highlyscalable_avx2     13892ms 719.839 9792
highlyscalable_avx512   15337ms 652.018 9792
lemire_highlyscalable_SIMD      24527ms 407.714 9792
scalar  69943ms 142.974 9792
block_merge     44386ms 225.296 9792
sttni   16300ms 613.497 9792
```
当数量级达到100w时，highlyscalable_avx2算法速度依然是最快的。

### 结论
推荐使用highlyscalable_avx2算法，它简单、不依赖额外内存空间且速度最快。
