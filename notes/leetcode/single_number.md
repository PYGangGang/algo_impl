# Single Number
---
这个题还挺有意思的，我感觉好多题都比较类似，来总结一下。
最经典的就是 [lc #136](https://leetcode-cn.com/problems/single-number/)：
>给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。
example:
输入: [2,2,1]
输出: 1

先说下此类题通用的暴力解法：
* 将数组排序，然后挨个比对
* 用一个 map 记录每个数字的出现次数，最后再遍历 map 找答案

而题目的小要求是不使用额外的空间，这时就祭出了位计算：我们将所有的元素按位异或，显然，根据异或相同为 0，不同为 1的规律，最后只会剩下只出现一次的那个数字；
```cpp
int singleNumber(vector<int>& nums) {
    int res = 0;
    for (auto i : nums) {
        res ^= i;
    }
    return res;
}
```
这里，我们就不再讨论是怎么想到用位运算来解决的了（因为我也不知道），且把这当作一个基础思想，继续往后看拓展题。

---
自然的，其他元素出现两次，也能出现三次；这个变种就是 [lc #137](https://leetcode-cn.com/problems/single-number-ii/)：
>给你一个整数数组 nums ，除某个元素仅出现 一次 外，其余每个元素都恰出现 三次 。请你找出并返回那个只出现了一次的元素。
example:
输入：nums = [2,2,3,2]
输出：3

暴力的思想都一样，不再赘述了，我们看下位计算的变化。很明显，光异或没啥用了，会得到每种数字按位异或的奇怪值；进而（暂且估计大家和我思想一样），我们用各种按位运算都试了一下，与、或、非、同或、异或算完都不能直接得到答案。但是直觉还是可以用位处理做，那就用例子先画一画：
位|2|2|3|2|ans: 3
-|-|-|-|-|-
1st|0|0|0|0|0
2nd|1|1|1|1|1
3rd|0|0|1|0|0
因为其他数都出现三次，则统计所有数每一位上 1 的个数，统计出来的值应该每一位 1 的个数都能被 3 整除，不能被整除的位说明答案数的对应位为 1，那就统计就好了：
```cpp
int singleNumber(vector<int>& nums) {
    int int_size = sizeof(int) * 8;
    vector<int> rec(int_size, 0);
    for (auto i : nums) {
        for (int j = 0; j < int_size; ++j) {
            int bit = (i >> j) & 1;
            rec[j] += bit;
        }
    }
    int res = 0;
    for (int i = 0; i < int_size; ++i) {
        res |= (rec[i] % 3) << i;
    }
    return res;
}
```
很自然，我们拿一个和 int 位数一样大的数组，数一下每位有几个 1 就行；当然，我们也可以不用额外的数组：
```cpp
int singleNumber(vector<int>& nums) {
    int int_size = sizeof(int) * 8;
    int res = 0;
    for (int i = 0; i < int_size; ++i) {
        int num = 0;
        for (auto n : nums) {
            num += (n >> i) & 1 ? 1 : 0;
        }
        res |= (num % 3 ? 1 : 0) << i;
    }
    return res;
}
```
把循环的顺序换一下，按位去数 1 就不要额外的数组了。但是，这样的时间复杂度实际上是 O(nlogC)，C指输入的数据的可能位数，此处为 32. 那必须 O(n) 解决啊，这暴脾气。