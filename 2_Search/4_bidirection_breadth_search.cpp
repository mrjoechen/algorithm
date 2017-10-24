//双向广度搜索
//bidirection_breadth_search.cpp

//在给定的矩阵空间中，从起始点开始搜索一条最短到达目标点的路径

//双向广度搜索方法是广搜法bfs的扩展
//双向广搜相比bfs占用内存少很多，而且总能保证得到的路径是最短路径
//
//具体实现
//使用两个队列(bfs只有一个队列)，分别存储搜索起始点和目标点
//分别从起始点和目标点进行广搜，同时保持两队列中存储的点数量一致
//这是为了防止两个队列中的点数量相差太大，从而退化成一个队列的bfs
//两个队列会在中途碰到同一个点，即找到一条从起点到终点的路径
//重复以下步骤：
//两个队列每次要新加入一个节点时，都需要判断该节点是否已经存在在另一个队列中
//若对方队列已经访问过这个点，则两个队列搜索到了相同的点，即两个队列在这个点碰面
//在每次新加入一个点时都需要记录下该节点的父节点，即将该节点加入队列的那个邻节点
//当两个队列碰面时，从碰面点沿着两个队列中的父节点指针就可以找回起点和终点
//该路径即所求最短路径
//
//本文引用了“双向广度优先搜索算法框架及八数码问题例程”，作者“~纯净的天空~”

#include "general_head.h"
#include "search.h"
pair<bi_node, bi_node> expand_queue(deque<bi_node>& q_expand,
		deque<bi_node>& q_exist, bi_node **s, int m, int n, int **visited, int flag);
void print_road(pair<bi_node, bi_node> meet_pos, bi_node **s);
void print_forward(bi_node f, bi_node **s);
void print_backward(bi_node b, bi_node **s);

void bidirection_breadth_search(bi_node **s,
		int m, int n, bi_node beg, bi_node end)
{//矩阵s有m行n列，行下标从0到m-1，列下标从0到n-1
	//visit_beg记录beg队列访问过的点
	//visit_end记录end队列访问过的点
	int **visit_beg = new int*[MAX];
	int **visit_end = new int*[MAX];
	for(int i = 0; i < m; ++ i){
		visit_beg[i] = new int[MAX];
		memset(visit_beg[i], 0, MAX * sizeof(int));
		visit_end[i] = new int[MAX];
		memset(visit_end[i], 0, MAX * sizeof(int));
	}
	//q_beg是从起点bfs的队列
	//q_end是从终点bfs的队列
	deque<bi_node> q_beg, q_end;
	//起点终点分别进入两个队列
	q_beg.push_back(beg), q_end.push_back(end);
	visit_beg[beg.b_row][beg.b_col] = 1, visit_end[end.b_row][end.b_col] = 1;
	//meet_pos返回q_beg和q_end中相遇的点
	//这两个点是相邻的，即这两个点相遇
	pair<bi_node, bi_node> meet_pos;
	int flag(0);
	while(1){
		if(q_beg.size() > q_end.size()){
			//扩展q_end，检查要加入q_end的点是否已存在于q_beg中
			//最后一个参数 2 标志被扩展的队列是q_end
			meet_pos = expand_queue(q_end, q_beg, s, m, n, visit_end, 2);
			if(meet_pos != pair<bi_node, bi_node>(*q_end.end(), *q_end.end())){
				//找到了相遇点
				flag = 2;
				break;
			}
		}
		else{
			//扩展q_beg，检查要加入q_beg的点是否已存在于q_end中
			//最后一个参数 1 标志被扩展的队列是q_beg
			meet_pos = expand_queue(q_beg, q_end, s, m, n, visit_beg, 1);
			if(meet_pos != pair<bi_node, bi_node>(*q_beg.end(), *q_beg.end())){
				flag = 1;
				break;
			}
		}
	}
	//若找到相遇的两点，则用print_road函数输出路径
	if(flag == 2)
		//若flag为2说明在扩展q_end队列时，待扩展的q_end节点与q_beg中某节点相遇
		//meet_pos.first是q_end队列中节点，meet_pos.second是q_beg队列中节点
		//交换位置，因为print_road函数将first看作q_beg中节点，second看作q_end中节点
		swap(meet_pos.first, meet_pos.second);
	print_road(meet_pos, s);
}
pair<bi_node, bi_node> expand_queue(deque<bi_node>& q_expand,
		deque<bi_node>& q_exist, bi_node **s, int m, int n, int **visited, int flag)
{//对q_expand队列的头节点的所有邻节点进行扩展
 //在将某个邻节点加入队列q_expand之前，先检查该点是否已经被q_expand队列访问过
 //或者查找该点是否已经存在于q_exist队列中
 //若已经存在，则q_expand的头节点与q_exist中该邻节点是相邻的，即两队列相遇
	deque<bi_node>::iterator pos;
	bi_node p = q_expand.front(); q_expand.pop_front();
	//扩展q_expand头节点的四个方向上的邻节点
	if(p.b_row - 1 >= 0 && !visited[p.b_row - 1][p.b_col]){
		//判断矩阵边界与是否已经被访问过	
		//C++算法find需要结构体bi_node提供比较操作operator==
		//在search.h中该操作操作定义两节点的xy坐标相等即为同一点，不考虑父节点指针
		if((pos = find(q_exist.begin(), q_exist.end(), s[p.b_row - 1][p.b_col])) !=
					q_exist.end())
			//若在q_exist中找到q_expand头节点的该邻节点，则返回相遇的两节点
			//返回的节点中first是q_expand队列的待扩展节点p，second是q_exist队列中节点
			return(pair<bi_node, bi_node>(p, *pos));
		//若没有在q_exist中找到该邻节点则两队列未在此节点处相遇
		//将q_expand队列扩展至该邻节点，并标记其父节点指针
		s[p.b_row - 1][p.b_col].b_fa.first = p.b_row;
		s[p.b_row - 1][p.b_col].b_fa.second = p.b_col;
		q_expand.push_back(s[p.b_row - 1][p.b_col]);
		visited[p.b_row - 1][p.b_col] = 1;
	}
	if(p.b_row + 1 < m && !visited[p.b_row + 1][p.b_col]){
		if((pos = find(q_exist.begin(), q_exist.end(), s[p.b_row + 1][p.b_col])) !=
					q_exist.end())
			return(pair<bi_node, bi_node>(p, *pos));
		s[p.b_row + 1][p.b_col].b_fa.first = p.b_row;
		s[p.b_row + 1][p.b_col].b_fa.second = p.b_col;
		q_expand.push_back(s[p.b_row + 1][p.b_col]);
		visited[p.b_row + 1][p.b_col] = 1;
	}
	if(p.b_col - 1 >= 0 && !visited[p.b_row][p.b_col - 1]){
		if((pos = find(q_exist.begin(), q_exist.end(), s[p.b_row][p.b_col - 1])) !=
					q_exist.end())
			return(pair<bi_node, bi_node>(p, *pos));
		s[p.b_row][p.b_col - 1].b_fa.first = p.b_row;
		s[p.b_row][p.b_col - 1].b_fa.second = p.b_col;
		q_expand.push_back(s[p.b_row][p.b_col - 1]);
		visited[p.b_row][p.b_col - 1] = 1;
	}
	if(p.b_col + 1 < n && !visited[p.b_row][p.b_col + 1]){
		if((pos = find(q_exist.begin(), q_exist.end(), s[p.b_row][p.b_col + 1])) !=
					q_exist.end())
			return(pair<bi_node, bi_node>(p, *pos));
		s[p.b_row][p.b_col + 1].b_fa.first = p.b_row;
		s[p.b_row][p.b_col + 1].b_fa.second = p.b_col;
		q_expand.push_back(s[p.b_row][p.b_col + 1]);
		visited[p.b_row][p.b_col + 1] = 1;
	}
	//一次扩展只是从q_expand队列的头节点向外扩展四个邻节点
	//若这次扩展中两队列未相遇则返回q_expand的尾部迭代器
	return(pair<bi_node, bi_node>(*q_expand.end(), *q_expand.end()));
}
void print_road(pair<bi_node, bi_node> meet_pos, bi_node **s)
{//meet_pos中是两队列相遇处的两节点
 //其中first是q_beg队列中的节点，second是q_end队列中的节点，这两个点是相邻的
 //输出路径时向后递归逆序输出q_beg的路径，向前顺序输出q_end的路径
	print_backward(s[meet_pos.first.b_row][meet_pos.first.b_col], s);
	print_forward(s[meet_pos.second.b_row][meet_pos.second.b_col], s);
}
void print_backward(bi_node b, bi_node **s)
{//从点b向起点反向输出路径
 //注意这里递归的使用，输出的顺序是从起点到终点的顺序
	if(b.b_fa.first != -1 && b.b_fa.second != -1)
		print_backward(s[b.b_fa.first][b.b_fa.second], s);		
	cout << " (row:" << b.b_row << ",col:" << b.b_col << ")" << endl;
}
void print_forward(bi_node f, bi_node **s)
{//从点f向终点输出路径
	cout << " (row:" << f.b_row << ",col:" << f.b_col << ")" << endl;
	if(f.b_fa.first != -1 && f.b_fa.second != -1)
		print_forward(s[f.b_fa.first][f.b_fa.second], s);
}
