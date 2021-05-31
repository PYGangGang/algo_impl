# 前缀和 / Prefix Sum

### 从 [LC #560 和为 K 的子数组](https://leetcode-cn.com/problems/subarray-sum-equals-k/) 出发来看下前缀和的思想。

> 给定一个整数数组和一个整数 **k，**你需要找到该数组中和为 **k** 的连续的子数组的个数。

如果我们要计算一个数组的每一个连续子数组的和，按照暴力的方法，我们需要依次确定开始下标和结束下标，然后计算每个子数组的和：

```cpp
int subarraySum(vector<int>& nums, int k) {
    int res = 0;
    for (int i = 0; i < nums.size(); ++i) {
    	for (int j = i; j < nums.size(); ++j) {
        	if (getSum(nums, i, j) == k) ++res; // 求和函数，这里不写了
        }
    }
    return res;
}
```

**很明显，这里是有冗余的**；例如一个子数组 `nums[2] -> nums[5]` 我们在 `1 -> 6` / `2 -> 7` ……很多个它的超集的求和中都重复计算了 `2 -> 5` 这一个子数组。

去除冗余计算的第一种方法是我们用一个二维的记录数组 `dp[i][j]` 来记录子数组 `i -> j` 的和，当要计算它的超集的时候，我们直接用这个已有的值来避免重复计算：

```cpp
int subarraySum(vector<int>& nums, int k) {
    vector<vector<int>> dp(nums.size(), vector<int>(nums.size()));
    int res = 0;
    for (int i = 0; i < nums.size(); ++i) {
        if (nums[i] == k) ++res;
        dp[i][i] = nums[i];
        for (int j = i + 1; j < nums.size(); ++j) {
            dp[i][j] = dp[i][j - 1] + nums[j];
            if (dp[i][j] == k) ++res;
        }
    }
    return res;
}
```

**这里的存储和计算，仍然是有冗余的，前缀和就很好的去除了这个冗余。**为了计算任意子数组的和，我们并不需要其所有子集，而只需要开始和结尾的前缀和。我们用一个 `pre[i]` 数组来记录 `0 -> i` 子数组的和（即前缀和），当我们需要 `i -> j` 子数组的和时，我们只需要 `pre[j] - pre[i - 1]` 即可得到，那么我们就可以进一步优化：

```cpp
int subarraySum(vector<int>& nums, int k) {
    vector<int> pre(nums.size());
    int res = 0;
    if (nums[0] == k) ++res;
    pre[0] = nums[0];
    for (int i = 1; i < nums.size(); ++i) {
        pre[i] = pre[i - 1] + nums[i];
        if (pre[i] == k) ++res;
        int target = pre[i] - k;
        for (int j = 0; j < i; ++j) {
            if (pre[j] == target) ++res;
        }
    }
    return res;
}
```

至此，前缀和及其思想就介绍完了：**用一次减法即可得到一个子数组的和**。

针对这道题，我们并不需要“符合条件的前缀和出现在哪个下标”这一确切的条件，所以我们可以进一步优化：用一个哈希表来存某个前缀和已出现的次数：

```cpp
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> m;
    m[0] = 1; // 一个数都没有的情况
    int res = 0;
    int pre = 0;
    for (int i = 0; i < nums.size(); ++i) {
        pre = nums[i] + pre;
        int target = pre - k;
        res += m[target]; // 这样写为了看起来简洁，find 一下效率更高
        ++m[pre];
    }
    return res;
}
```

---

### [LC #1074 元素和为目标值的子矩阵数量](https://leetcode-cn.com/problems/number-of-submatrices-that-sum-to-target/) 将前缀和扩展到二维矩阵

> 给出矩阵 `matrix` 和目标值 `target`，返回元素总和等于目标值的非空子矩阵的数量。

找子数组和找子矩阵其实是非常类似的。我们可以枚举矩阵的上下边界，将每一列的元素求和（归一）后将一个矩阵变成一个数组，进而去找数组中的目标值。而求和每一列，我们会很自然想到每枚举一个上下边界去做一次累加计算；其实这里可以**再用一次前缀和**，通过上一个枚举到的下边界，计算得到最新的列和：

```cpp
int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
    int res = 0;
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 0; i < m; ++i) {				// 枚举上边界
        vector<int> nums(n);
        for (int j = i; j < m; ++j) {			// 枚举上边界
            for (int k = 0; k < n; ++k) {		// 每一列的前缀和数组
                nums[k] += matrix[j][k];
            }
            res += numSubarraySumTarget(nums, target);
        }
    }
    return res;
}

int numSubarraySumTarget(vector<int> &nums, int k) {
    unordered_map<int, int> m;
    m[0] = 1;
    int res = 0, pre = 0;
    for (int i : nums) {
        pre += i;
        int target = pre - k;
        if (m.find(target) != m.end()) res += m[target];
        ++m[pre];
    }
    return res;
}
```

---

### [LC #523 连续的子数组和](https://leetcode-cn.com/problems/continuous-subarray-sum/) 前缀和数组也可以变一变

> 给定一个包含 非负数 的数组和一个目标 整数 k ，编写一个函数来判断该数组是否含有连续的子数组，其大小至少为 2，且总和为 k 的倍数，即总和为 n * k ，其中 n 也是一个整数。
>

如果不是找和为 k，而是找和为 k 的倍数的子数组，那么我们去存前缀和（目的是为了求得子数组的和）就不再高效了。此处我们可以存前缀和对 k 取余的值，若子数组 `i -> j` 的和为 k 的倍数，那么 `pre[i - 1] % k == pre[j] % k` （因为这俩一相减，就只剩一个 `k * 系数` 了。对于这道题需要稍微注意一下 `k * 0 = 0` 和 大小至少为 2 这两个条件：

```cpp
bool checkSubarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> m; // 需要存下某前缀和第一次出现的下标来确定组成的子数组大小不小于 2
    int pre = 0;
    for (int i = 0; i < nums.size(); ++i) {
        pre += nums[i];
        int rem = pre % k;
        if (rem == 0 && i >= 1) return true;
        if (m.find(rem) != m.end()) {
            if (i - m[rem] >= 2) return true;
        } else m[rem] = i;
    }
    return false;
}
```

我们可以发现，**前缀和提供的关键能力是方便地获得某个子数组的某种性质**，而不局限于它的和。

---

### 前缀和与异或运算

涉及到异或运算的地方是前缀和发挥威力的最好的地方，甚至我们看到异或就应该想到前缀和。异或有以下性质：

* `x ^ x == 0` -> `(x1 ^ x2 ^ x3) ^ (x1 ^ x2) == x3`
* 记前缀和数组 `pre[i]` 为 `0 -> i` 子数组的异或，那么任意子数组 `i -> j` 的异或值为 `pre[i - 1] ^ pre[j]`

