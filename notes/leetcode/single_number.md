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

把循环的顺序换一下，按位去数 1 就不要额外的数组了。但是，这样的时间复杂度实际上是 O(nlogC)，C 指输入的数据的可能位数，此处为 32. 那必须 O(n) 解决啊，这暴脾气。

这里和一次遍历的区别在于，我们对每一位都完整地遍历了一遍数组，有没有办法，只遍历一遍，计算结果能体现每一位上 1 出现的次数是否是 3 的倍数呢？

自然，我们可以想到用有限状态机。抽出每一位来看，遍历过程中，我们用一个“数据结构”来记录 1 出现的次数，因为我们只需要一个和 3 的倍数相关的东西，所以这个“数据结构”只需要在 0 -> 1 -> 2 -> 0 之间变化。

很明显，只用一个二进制数没法满足这个状态转移，而两个数就可以了。假设当前遇到的数是 x ，我们用 x<sub>i</sub> 来表示 x 的第 i 位：

a<sub>i</sub>|b<sub>i</sub>|x<sub>i</sub>|new a<sub>i</sub>|new b<sub>i</sub>
-|-|-|-|-
0|0|0|0|0
0|0|1|0|1
0|1|0|0|1
0|1|1|1|0
1|0|0|1|0
1|0|1|0|0

我们只需要让 a, b 的每一位，随着新遍历到的 x 中的每一位，按照这个表“蕴含”的规律变就行了；最后，因为其他数都出现了 3 次，所以 b 就会是最终的答案。

这就是一个简单的真值表，而每一位都遵守一样的逻辑，所以很简单地写出逻辑算式：

```cpp
a_new = (~a & b & x) | (a & ~b & ~x)
b_new = (~a & ~b & x) | (~a & b & ~x)
```

```cpp
int singleNumber(vector<int>& nums) {
    int a = 0, b = 0;
    for (auto n : nums) {
        int tmp_a = (~a & b & n) | (a & ~b & ~n);
        int tmp_b = (~a & ~b & n) | (~a & b & ~n);
        a = tmp_a;
        b = tmp_b;
    }
    return b;
}
```

当然，真值表的方法，也可以用于第一道题，不过就大题小作了。

---

再升级一下，带来一些新的思想 [lc #260](https://leetcode-cn.com/problems/single-number-iii/)：

>给定一个整数数组 nums，其中恰好有两个元素只出现一次，其余所有元素均出现两次。 找出只出现一次的那两个元素。你可以按 任意顺序 返回答案。  
example:  
输入：nums = [1,2,1,3,2,5]  
输出：[3,5]  

这道题没法用真值表了，那就先异或吧。称出现一次的数为 a, b，则我们全部异或的结果 x 就为 a ^ b；这个结果有什么意义呢？我们深入到每一位来看。如果 x 的第 i 位为 1 ，则表示 a 和 b 在第 i 位是不同的。太好了，我们直接按第 i 位进行一个分组，为 1 的分到一组，为 0 的分到一组。这样我们既保证了 a 和 b 分属于两组，也保证了每组中的其他数成对出现。这样就又回到第一题了。

```cpp
vector<int> singleNumber(vector<int>& nums) {
    int x = 0;
    for (auto i : nums) {
        x ^= i;
    }
    int div = 1;
    while ((x & div) == 0) {
        div <<= 1;
    }
    int a = 0, b = 0;
    for (auto i : nums) {
        if ((div & i) == 0) a ^= i;
        else b ^= i;
    }
    return vector<int>({a, b});
}
```