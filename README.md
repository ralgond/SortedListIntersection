# SortedListIntersection
搜集有序列表求交集算法

# 如何运行
make

./main docid_list_s.txt docid_list_c.txt

结果有3列，第一列是算法名，第二列是花费时间，第三列是交集大小。比如下面：
```
v1_avx2	186ms	6
v3_avx2	347ms	6
simple_merge	352ms	6
block_merge	454ms	6
bisearch	4110ms	6
bloom	3926ms	6
```
可以看出v1算法在众算法中最快，而bloom过滤器慢得出乎意料。
