# SortedListIntersection
搜集有序列表求交集算法

# 如何运行
make

./main data/docid_list_s.txt data/docid_list_c.txt

# 性能
### 数据集1（数量为2w）
./main data/docid_list_s.txt data/docid_list_c.txt

结果有3列，第一列是算法名，第二列是花费时间，第三列是交集大小。比如下面：
```
v1_avx2	189ms	6
v3_avx2	360ms	6
highlyscalable_SIMD	153ms	6
lemire_highlyscalable_SIMD	181ms	6
scalar	149ms	6./main data/10w_1.txt data/10w_2.txt 
block_merge	626ms	6
bisearch	3899ms	6
bloom	3771ms	6
```
可以看出scalar算法(最简单的算法)在众算法中最快，而bloom过滤器慢得出乎意料。

### 数据集2（数量为10w）
./main data/10w_1.txt data/10w_2.txt 
```
v1_avx2	1868ms	8944
v3_avx2	3119ms	8944
highlyscalable_SIMD	1789ms	8944
lemire_highlyscalable_SIMD	1918ms	8944
scalar	7589ms	8944
block_merge	5068ms	8944
bisearch	32771ms	8944
bloom	40660ms	8944
```
可以看出[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法在众算法中最快。
