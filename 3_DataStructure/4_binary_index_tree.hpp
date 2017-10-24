//树状数组
//binary_index_tree.hpp

//求数组s的所有成员的和

//求数组s的所有成员之和的原始方法是累加该范围中的每个成员
//若数组s有n个成员，则该方法的时间复杂度是O(n)
//也可以使用线段树来解决该问题，但最适合的是使用树状数组，可以将时间复杂度降到O(lgn)
//
//设置辅助数组c来实现树状数组数据结构
//应用树状数组的特殊条件是数组s和c的下标必须从1开始
//树状数组中每个节点为c[i]
//并且满足c[i] = s[i-2^k+1] + s[i-2^k+2] + ... + s[i]
//其中k是i在二进制形式下末尾的0的个数，且i >= 1
//用二进制的位运算可以轻松的得到2^k = i & (-i)
//
//比如i = 0010 1100 1000的末尾有3个0，则k = 3
//比如i从1到8时，有：
//i = 1，k = 0，2^k = 1
//i = 2，k = 1，2^k = 2
//i = 3，k = 0，2^k = 1
//i = 4，k = 2，2^k = 4
//i = 5，k = 0，2^k = 1
//i = 6，k = 1，2^k = 2
//i = 7，k = 0，2^k = 1
//i = 8，k = 3，2^k = 8
//i = 9，k = 0，2^k = 1
//i = 10，k = 1，2^k = 2
//i = 11，k = 0，2^k = 1
//i = 12，k = 2，2^k = 4
//i = 13，k = 0，2^k = 1
//i = 14，k = 1，2^k = 2
//i = 15，k = 0，2^k = 1
//i = 16，k = 4，2^k = 16
//则树状数组中的数组c和s有如下的对应关系：
//c[1] = s[1]
//c[2] = s[1] + s[2]
//c[3] = s[3]
//c[4] = s[1] + s[2] + s[3] + s[4]
//c[5] = s[5]
//c[6] = s[5] + s[6]
//c[7] = s[7]
//c[8] = s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7] + s[8]
//c[9] = s[9]
//c[10] = s[9] + s[10]
//c[11] = s[11]
//c[12] = s[9] + s[10] + s[11] + s[12]
//c[13] = s[13]
//c[14] = s[13] + s[14]
//c[15] = s[15]
//c[16] = s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7] + s[8] + s[9] + s[10] + s[11] + s[12] + s[13] + s[14] + s[15] + s[16]
//
//计算数组s中从1到n所有成员之和，或者改变某个成员的值时
//利用数组c和s在i和k上的对应关系可以很快的计算

#include "general_head.h"

struct binary_index_tree{
private:
	int b_c[MAX];		//数组c，两数组下标从1开始
	int b_lowbit(int i){
		//计算2^k = i & (-i)
		return(i & (-i));
	}
public:
	binary_index_tree(){
		memset(b_c, 0, MAX * sizeof(int));
	}
	void b_add(int i, int x){
		//s[i]加x，注意i从1开始
		while(i < MAX){
			//比如当i=1时，因为c[1]=s[1]故c[1]加x
			//lowbit(1)=1，i=2，恰好c[2]=s[1]+s[2]，c[2]中包含s[1]，因此c[2]加x
			//lowbit(2)=2，i=4，恰好c[4]=s[1]+s[2]+s[3]+s[4]，因此c[4]加x
			//lowbit(4)=4，i=8，恰好c[8]中包含s[1]，因此c[8]加x，以此类推
			//可以看出应用二进制位数的关系可以快速的改变数组s中的一个成员
			//而不必实际的设置数组s，只需要设置数组c
			b_c[i] += x;
			i += b_lowbit(i);
		}
	}
	int b_sum(int i){
		//计算数组s中从1到i的和
		//与加操作类似，避免了遍历从1到i的累加，从而降低时间复杂度
		int sum(0);
		while(i > 0){
			sum += b_c[i];
			i -= b_lowbit(i);
		}
		return(sum);
	}
	void b_print(){
		cout << endl << "array c: " << endl;
		for(int i = 1; i < MAX; ++ i){
			if(i % 10 == 0)
				cout << endl;
			cout << b_c[i] << '\t';
		}
		cout << endl;
	}
};
