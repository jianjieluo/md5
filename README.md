@(web安全)[homework]

# Assignment 2

MD5 即 Message-Digest Algorithm 5 (信息-摘要算法 5)
MD4 (1990)、MD5(1992, RFC 1321) 作者 Ron Rivest,是广泛使用的散列算法,**经常用于确保信息传输的完整性和一致性。**MD5 使用 little-endian,输入任意不定长度信息,以512位长进行分组,生成四个32位数据,最后联合起来输出固定128位长的信息摘要。MD5 算法的基本过程为:求余、取余、调整长度、与链接变量进行循环运算、得出结果。
- MD5 不是足够安全的
	- Hans Dobbertin 在1996年找到了两个不同的 512-bit 块,它们在 MD5 计算下产生相同的 hash 值。

MD5是一个hash算法，它通过吧任意长度的消息哈希成一个128位长的字符串，但是人们很难将这128位的字符串解出原来的字符串，因为有很多种组合的方式，而且在大部分的情况来看，至今还没有真正找到两个不同的消息,它们的 MD5 的 hash值相等。所以说它是可以保护密码的安全的。


## 1 Design an executable MD5 program in C/C++ with some short samples.

已经在根目录下实现了基本的md5的cpp实现。实验的环境是`Ubuntu 16.04`，简单的测试方法是直接`bash ./run.sh`即可，TA也可以做自行的改变。实现了一个`MD5`的类，直接的使用方法是：

```cpp
string input;
cin >> input;
MD5 md5(input);
cout << md5.getDigest() << endl;
```

## 2 Show me how MD5 works for password protection.

具体的参考资料是[RCF 1321](https://www.ietf.org/rfc/rfc1321.txt)

MD5算法的实现过程是有5个步骤

### 1. Append Padding Bits

在原始消息数据尾部填充标识 100...0,填充后的消息位数 L = 448 (mod 512)。至少要填充1个位,所以标识长度 1~512位。注意的是padding完之后要对每一个word做一个小端规则的重排，也就是说对于一个每一个word中的块"1234"，小端规则重排之后应该biancheng"4321""

### 2. Append Length
对于原始信息（不包括第一步padding补充的位数）的位数长度b，化成二进制表示后选取低64位，每个word按照小端规则添加到第一步处理后的消息数据的尾部。举例子来说，对于消息"a"，消息长度应该是8（8位代表一个字符串），所以二进制表示是"0000...0001000", 接着补充的情况是"0000...1000 000...000(32个0)"

### 3. Initialize MD Buffer
初始化4个32位的buffer：ABCD，最后的结果将是从ABCD中合起来128位输出。根据RCF的规定，按照小端规则存储ABCD

```
word A: 01 23 45 67
word B: 89 ab cd ef
word C: fe dc ba 98
word D: 76 54 32 10
```

则实际中的赋值就应该是：

```
A = 0x67452301;
B = 0xefcdab89;
C = 0x98badcfe;
D = 0x10325476;
```

### 4. Process Message in 16-Word Blocks
消息数据的二进制表示长度现在应该是512的倍数了，假设有L个512位的块，那么就应该做L次`transform`，每次`transform`按顺序输入消息数据的512位。

每次`transform`使用RCF上给出的宏，不难写出：

```cpp
void MD5::transform(int beginIndex) {
    bit32 AA = A, BB = B, CC = C, DD = D;

    bit32 x[16];
    decode(512 * beginIndex, x);

    /* Round 1 */
    FF(A, B, C, D, x[0], S11, 0xd76aa478);
    FF(D, A, B, C, x[1], S12, 0xe8c7b756);
    FF(C, D, A, B, x[2], S13, 0x242070db);
    FF(B, C, D, A, x[3], S14, 0xc1bdceee);
    FF(A, B, C, D, x[4], S11, 0xf57c0faf);
    FF(D, A, B, C, x[5], S12, 0x4787c62a);
    FF(C, D, A, B, x[6], S13, 0xa8304613);
    FF(B, C, D, A, x[7], S14, 0xfd469501);
    FF(A, B, C, D, x[8], S11, 0x698098d8);
    FF(D, A, B, C, x[9], S12, 0x8b44f7af);
    FF(C, D, A, B, x[10], S13, 0xffff5bb1);
    FF(B, C, D, A, x[11], S14, 0x895cd7be);
    FF(A, B, C, D, x[12], S11, 0x6b901122);
    FF(D, A, B, C, x[13], S12, 0xfd987193);
    FF(C, D, A, B, x[14], S13, 0xa679438e);
    FF(B, C, D, A, x[15], S14, 0x49b40821);

    /* Round 2 */
    GG(A, B, C, D, x[1], S21, 0xf61e2562);
    GG(D, A, B, C, x[6], S22, 0xc040b340);
    GG(C, D, A, B, x[11], S23, 0x265e5a51);
    GG(B, C, D, A, x[0], S24, 0xe9b6c7aa);
    GG(A, B, C, D, x[5], S21, 0xd62f105d);
    GG(D, A, B, C, x[10], S22, 0x2441453);
    GG(C, D, A, B, x[15], S23, 0xd8a1e681);
    GG(B, C, D, A, x[4], S24, 0xe7d3fbc8);
    GG(A, B, C, D, x[9], S21, 0x21e1cde6);
    GG(D, A, B, C, x[14], S22, 0xc33707d6);
    GG(C, D, A, B, x[3], S23, 0xf4d50d87);
    GG(B, C, D, A, x[8], S24, 0x455a14ed);
    GG(A, B, C, D, x[13], S21, 0xa9e3e905);
    GG(D, A, B, C, x[2], S22, 0xfcefa3f8);
    GG(C, D, A, B, x[7], S23, 0x676f02d9);
    GG(B, C, D, A, x[12], S24, 0x8d2a4c8a);

    /* Round 3 */
    HH(A, B, C, D, x[5], S31, 0xfffa3942);
    HH(D, A, B, C, x[8], S32, 0x8771f681);
    HH(C, D, A, B, x[11], S33, 0x6d9d6122);
    HH(B, C, D, A, x[14], S34, 0xfde5380c);
    HH(A, B, C, D, x[1], S31, 0xa4beea44);
    HH(D, A, B, C, x[4], S32, 0x4bdecfa9);
    HH(C, D, A, B, x[7], S33, 0xf6bb4b60);
    HH(B, C, D, A, x[10], S34, 0xbebfbc70);
    HH(A, B, C, D, x[13], S31, 0x289b7ec6);
    HH(D, A, B, C, x[0], S32, 0xeaa127fa);
    HH(C, D, A, B, x[3], S33, 0xd4ef3085);
    HH(B, C, D, A, x[6], S34, 0x4881d05);
    HH(A, B, C, D, x[9], S31, 0xd9d4d039);
    HH(D, A, B, C, x[12], S32, 0xe6db99e5);
    HH(C, D, A, B, x[15], S33, 0x1fa27cf8);
    HH(B, C, D, A, x[2], S34, 0xc4ac5665);

    /* Round 4 */
    II(A, B, C, D, x[0], S41, 0xf4292244);
    II(D, A, B, C, x[7], S42, 0x432aff97);
    II(C, D, A, B, x[14], S43, 0xab9423a7);
    II(B, C, D, A, x[5], S44, 0xfc93a039);
    II(A, B, C, D, x[12], S41, 0x655b59c3);
    II(D, A, B, C, x[3], S42, 0x8f0ccc92);
    II(C, D, A, B, x[10], S43, 0xffeff47d);
    II(B, C, D, A, x[1], S44, 0x85845dd1);
    II(A, B, C, D, x[8], S41, 0x6fa87e4f);
    II(D, A, B, C, x[15], S42, 0xfe2ce6e0);
    II(C, D, A, B, x[6], S43, 0xa3014314);
    II(B, C, D, A, x[13], S44, 0x4e0811a1);
    II(A, B, C, D, x[4], S41, 0xf7537e82);
    II(D, A, B, C, x[11], S42, 0xbd3af235);
    II(C, D, A, B, x[2], S43, 0x2ad7d2bb);
    II(B, C, D, A, x[9], S44, 0xeb86d391);

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
}
```

其中因为要把输入的512位的数据part分成32位的块进行信息摘要，所以使用`decode()`方法来把512位的数据part分发到一个`x[16]`数组中，数组中的每个元素是32位长的数据单元（其实就是用int来表示）。

### 5. Output

进行完第4步之后，ABCD中存的数据就是md5提取出来信息了，现在我们要把它按照ABCD的顺序，使用大端规则用字符的形式表示出来。这里使用`snprintf`的方法。每8位每8位地取ABCD中二进制数据然后转成字符最后输出。

```cpp
const string MD5::to_str() {
    // Output in Big-Endian
    // For a value 0x12345678
    // Output as    "78563412"
    bit32 input[4];
    input[0] = A;
    input[1] = B;
    input[2] = C;
    input[3] = D;

    string ret;
    char buffer[4];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            snprintf(buffer, 4, "%02x", input[i] >> j * 8 & 0xff);
            ret += buffer;
        }
    }
    return ret;
}
```

### 总结

根据RCF的思路来写其实并不是困难，主要需要注意的还是小端规则等的坑，我也是通过尝试以及和已有的线上md5运算器来对比才弄清楚了padding和append length中的主要细节。另外需要注意的是数据的表示类型的选择，不同的选择会对编程量有不同的影响。我一开始使用的是`vector<bool>`的方式来存储消息数据，这样的好处的是padding和append length的操作非常直观简单，但是给消息分组和小端规则的转换方面带来了一些麻烦，使用特定的方法来解决即可。

## 主要参考资料

[RCF 1321](https://www.ietf.org/rfc/rfc1321.txt)
[md5的一个cpp实现](https://github.com/JieweiWei/md5)