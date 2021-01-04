# SortedListIntersection
搜集有序列表求交集算法

# 如何运行
make

./main docid_list_s.txt docid_list_c.txt

结果有3列，第一列是算法名，第二列是花费时间，第三列是交集大小。比如下面：
```
v1_avx2	187ms	6
v3_avx2	346ms	6
highlyscalable_SIMD	155ms	6
lemire_highlyscalable_SIMD	160ms	6
simple_merge	355ms	6
block_merge	403ms	6
bisearch	4144ms	6
bloom	3878ms	6
```
可以看出[highlyscalable_SIMD](http://highlyscalable.wordpress.com/2012/06/05/fast-intersection-sorted-lists-sse/)算法在众算法中最快，而bloom过滤器慢得出乎意料。
