// H.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SCALE 128
#define CODELEN 30

template <typename E, typename Comp>
class Heap {
private:
	E* heap;
	int curSize;
	int maxSize;
	//定义一个辅助函数使得元素位于正确的位置 
	void shiftDown(int pos) {
		while (isLeaf(pos)) {
			int small = leftChild(pos); //假设左子节点值较小
			int rc = rightChild(pos);
			if ((rc < curSize), compare(small, rc))
				small = rc;
			if (compare(pos, small))
				swap(pos, small);
			pos = small;
		}
	}
	void swap(int one, int two)
	{
		E it = heap[one];
		heap[one] = heap[two];
		heap[two] = it;
	}
	bool compare(int one, int two) {
		return Comp::priority(heap[one], heap[two]);
	}
public:
	Heap(E* it, int num, int max)
	{
		heap = it;
		curSize = num;
		maxSize = max;
		buildHeap();
	}
	int size() const {
		return curSize;
	}
	bool isLeaf(int pos)
	{
		return (pos >= curSize / 2) && (pos < curSize);
	}
	int leftChild(int pos)
	{
		return 2 * pos + 1;
	}
	int rightChild(int pos)
	{
		return 2 * pos + 2;
	}
	int parent(int pos) {
		return (pos - 1) / 2;
	}
	void buildHeap() {
		for (int i = (curSize / 2 - 1); i >= 0; i--)
		{
			shiftDown(i);
		}
	}

	void insert(const E& it)
	{
		if (curSize >= maxSize)
		{
			printf("insert error,heap is full");
			exit(-1);
		}
		int curr = curSize++;
		heap[curr] = it;
		while ((curr != 0) && compare(curr, parent(curr)))
		{
			swap(curr, parent(curr));
			curr = parent(curr);
		}

	}

	E removeFirst() {
		if (curSize < 0) {
			printf("removefirst error,heap is empty");
		}

		swap(0, --curSize);
		if (curSize != 0)
			shiftDown(0);
		return heap[curSize];
	}

	E remove(int pos) {
		if ((pos < 0) || (pos >= 0)) {
			printf("remove error, bad position");
		}
		if (pos = n - 1)
			n--; //要删除的是最后一位
		else {
			swap(pos, --curSize);
			while ((pos != 0) || compare(pos, parent(pos))) {
				swap(pos, parent(pos));
				pos = parent(pos);
			}
			if (n != 0)
				shiftDown(pos);
		}
		return heap[curSize];
	}
};


typedef struct {
	char ch;
	int frequency;
	char* code;
}node;

class HuffmanNode {
private:inline bool operator>=(HuffmanNode&);
		inline bool operator<=(HuffmanNode&);
public:
	virtual ~HuffmanNode() {};
	virtual int getWeight() = 0;
	virtual bool isLeaf() = 0;

};
class leaf :public HuffmanNode {
private:
	char it;
	int weight;

public:
	leaf(node& item)
	{
		it = item.ch;
		weight = item.frequency;
	}
	leaf(char ch, int frequency) {
		it = ch;
		weight = frequency;
	}

	int getWeight() {
		return weight;
	}

	char value() {
		return it;
	}

	bool isLeaf() {
		return true;
	}
};

class intNode :public HuffmanNode {
private:
	int weight;
	HuffmanNode* lc;
	HuffmanNode* rc;
public:
	intNode() {
		weight = 0;
	}
	intNode(HuffmanNode* l, HuffmanNode* r) {
		lc = l;
		rc = r;
		weight = l->getWeight() + r->getWeight();
	}

	int getWeight()
	{
		return weight;
	}

	bool isLeaf() {
		return false;
	}

	HuffmanNode* getRightChild() {
		return rc;
	}

	void setRightChild(HuffmanNode* item) {
		rc = item;
	}

	HuffmanNode* getLeftChild()
	{
		return rc;
	}

	void setLeftChild(HuffmanNode* item) {
		lc = item;
	}
};

class HuffTree {
private:
	HuffmanNode* root;
public:
	HuffTree() {

	}
	HuffTree(char ch, int fre)
	{
		root = new leaf(ch, fre);
	}

	HuffTree(node item) {
		root = new leaf(item);
	}
	HuffTree(HuffTree* l, HuffTree* r) {
		root = new intNode(l->getRoot(), r->getRoot());
	}
	~HuffTree() {
	}
	HuffmanNode* getRoot() { return root; }
	int getWeight() {
		return root->getWeight();
	}



};

//比较类，对比哪个huffftree权重小
class compare {
public:
	static bool priority(HuffTree* one, HuffTree* two) {
		return one->getWeight() > two->getWeight();
	}
};

inline bool HuffmanNode::operator<=(HuffmanNode& it)
{
	return this->getWeight() <= it.getWeight();
}
inline bool HuffmanNode::operator>=(HuffmanNode& it)
{
	return this->getWeight() >= it.getWeight();
}
void generateCode(node* arr, FILE* file,HuffTree* tree);

bool readLine(FILE* file, std::string line)
{
	unsigned char ch;
	while ((ch = fgetc(file)) != EOF && ch != '\n')
	{
		line += ch;
		if (feof(file))
		{
			return false;
		}
	}
	return true;
}

//统计一行数据中各个char的出现频率
void count(int* sta, std::string line) {
	int len = line.length();
	for (int i = 0; i < len; i++)
	{
		char ch = line.at(i);
		sta[ch] ++;
	}

	//return sta;
}


//打印出字符串中出现的字符出现了多少次
void display(int* times)
{
	for (int i = 0; i < SCALE; i++)
	{
		if (times[i] != 0)
			printf("%c occurs %d times\n", i, times[i]);
	}
}


//生成一个符合要求的hufftree
HuffTree* buildHuffTree(HuffTree** treeArr, int scale) {
	Heap<HuffTree*, compare>* forest = new Heap<HuffTree*, compare>(treeArr, scale, scale);
	HuffTree* temp1, *temp2, *temp3 = NULL;
	while (forest->size() > 1) {
		temp1 = forest->removeFirst();
		temp2 = forest->removeFirst();
		temp3 = new HuffTree(temp1, temp2);
		forest->insert(temp3);
		delete temp1;
		delete temp2;
	}
	return temp3;
}

void run();

int main()
{
	run();
	
}



void encode(node* arr,char* content,HuffmanNode* root,int level =0) 
{
	if (root->isLeaf())
	{
		char item = ((leaf*)root)->value();
		int i = 0;
		while (arr[i++].ch != item);
		//strcpy(arr[i - 1].code, content);
		strcpy_s(arr[i-1].code,strlen(content)+1,content);
	}
	else {
		content[level] = '0';
		content[level + 1] = '\0';
		encode(arr, content, ((intNode*)root)->getLeftChild(), level+1);
		content[level] = '1';
		content[level + 1] = '\0';
		encode(arr, content, ((intNode*)root)->getRightChild(), level + 1);
	}
}
void generateCode(node* arr,FILE* file, HuffTree* tree)
{
	std::string str;
	readLine(file, str);
	int std[SCALE] = { 0 };
	count(std, str);//将各个字符的出现频率写入std数组
	//构造对应的node节点
	int order = 0;
	for (int i = 0; i < SCALE; i++)
	{
		arr[i].frequency = 0;
		if (std[i] != 0)
		{
			node curr;
			curr.ch = i;
			curr.frequency = std[i];
			arr[i] = curr;
			order++;
		}
	}
  //构造相应的hufffmantree
	HuffTree* forest[SCALE];
	int j = 0;
	for (int i = 0; i < SCALE; i++)
	{
		if (arr[i].frequency != 0)
		{
			forest[j++] = new HuffTree(arr[i].ch, arr[i].frequency);
		}
	}
	tree = buildHuffTree(forest,order);
	char content[CODELEN];
	encode(arr, content, tree->getRoot(), 0);
	
}
//写入相应的配置信息，将树序列化用顺序表示法表示，将叶节点
//对应的字符写入文件表示叶节点，相应的内部节点写入‘/’,是用完全二叉树的方式存储
//huffman编码树
void writeConfigure(FILE* file, HuffmanNode* root)
{
	if (root->isLeaf())
	{
		fputc(((leaf*)root)->value(), file);
		return;
	}
	else {
		fputc('/', file);
		writeConfigure(file,((intNode*)root)->getRightChild());
		writeConfigure(file, ((intNode*)root)->getLeftChild());
	}
}
//在arr中写入文件中各个字符的出现频率
void read(FILE* file, node* arr)
{
	int std[SCALE] = { 0 };
	std::string line;
	while (feof(file))
	{
		readLine(file, line);
		count(std, line);
	}
	for (int i = 0; i < SCALE; i++)
	{
		arr[i].frequency = std[i];
	}
}
bool compress(char* fileName)
{
	FILE* reader;
	if (!fopen_s(&reader, fileName, "rt+"))
	{
		printf("error, file does not exit ,please input correct filepath");
		return false;
	}
	//创建新文件储存压缩文本
	FILE* result;
	fopen_s(&result, "compress.zip", "rt+"); 
	node arr[SCALE];
	//初始化arr
	for (int i = 0; i < SCALE; i++)
	{
		arr[i].code = new char[CODELEN + 1];
		for (int j = 0; j <= CODELEN; j++)
		{
			arr[i].code[j] = '-'; //暂时全部初始化为零
		}
		arr[i].code[CODELEN] = '\0';
		arr[i].frequency = 0;
	}
	read(reader, arr);
	HuffTree* tree;
	tree = new HuffTree();
	generateCode(arr, reader,tree);
	FILE* conf;
	fopen_s(&conf,"congig.txt", "rt+"); //创建配置文件
	//写入配置文件
	writeConfigure(conf, tree->getRoot());
	//写入编码
	fclose(conf);
	char ch;
	while (ch = fgetc(reader) != EOF)
	{
		int index = 0;
		//if (ch == '\n')
			//fputc('\n', result);
		while (arr[index++].ch == ch);//查找对应字符的编码
		//fputs(arr[index - 1].code, result);
		//fputc(' ', result);
		fprintf(result, "%s", arr[index - 1].code);
		//编码不足八位补足八位
		int len = 8 - (strlen(arr[index - 1].code));
		while (len > 0) {
			len--;
			fputc('0',result);
		}
	}

	fclose(reader);
	fclose(result);
	return true;
}
//读取配置信息，重构hufffmantree
HuffmanNode* readConfigure(FILE* file)
{
	char ch = fgetc(file);
	if ((ch != '\0') && (ch != '/'))
	{
		return new leaf(ch, 0);
	}
	else {
		intNode* root;
		root = new intNode();
		root->setLeftChild(readConfigure(file));
		root->setRightChild(readConfigure(file));
		return root;
	}

}
//获取node
bool getNode(node* arr,FILE* file)
{
	char  ch;
	ch = fgetc(file);
	if (ch != '@')
	{
		printf("error,configure imfomation damage");
		return false;
	}
	ch = fgetc(file);
	ch = fgetc(file);//读取第一个‘/’代表了第一个内部节点
	int order = 0;
	intNode*  parent;
	parent = new intNode();
	bool left = true; //标志是不是左子节点
	while (true)
	{
		ch = fgetc(file);
		  if (ch == '/')
		   {
			  intNode* inner;
			  inner = new intNode();
			  parent->setLeftChild((HuffmanNode* )inner);
			  parent = inner;
			  left = false;
		  }
		  else
		  {
			  leaf l(ch, 0);

		  }
	
		//读取一个数据生成node节点	
	}//while
	return true;
}
//比较两个字符串的前八位是否相同
bool equal(char* one, char* two)
{
	if (one == NULL || one == NULL)
		return false;
	else
		for (int i = 0; i < 8; i++)
		{
			if (one[i] != two[i])
			{
				return false;
			}
		}
	return true;
}
bool uncompress(  char* fileName)
{
	FILE* file;
	if (!fopen_s(&file, fileName, "r"))
	{
		printf("error, file does not exit ,please input correct filepath");
		return false;
	}
	node arr[SCALE];
	for (int i = 0; i < SCALE; i++)
	{
		arr[i].code = new char[CODELEN + 1];
		for (int j = 0; j <= CODELEN; j++)
		{
			arr[i].code[j] = '-'; //暂时全部初始化为零
		}
		arr[i].code[CODELEN] = '\0';
		arr[i].frequency = 0;
		arr[i].ch = (char)i;
	}
	HuffmanNode* tree;
	//读取配置，生成编码树
	FILE* conf;
     fopen_s(&conf,"congig.txt", "r");
	tree = readConfigure(conf);
	fclose(conf);
	char str[SCALE];
	//产生编码
	encode(arr, str, tree, 0);
	//补足八位编码
	for (int i = 0; i < SCALE; i++)
	{
		int l = strlen(arr[i].code);
		if (l < 8)
		{
			for (int j = l; j < 8; j++)
				arr[i].code[j] = '0';
		}
	}
	//开始译码，创建一个新的文件并写入
	FILE* fp;
	fopen_s(&fp,"new.txt", "rt+");
	char ch[10];
	int len = sizeof(char)*8+1;
	fgets(ch,len,file);
	int index=0;
	while (feof(file))
	{
		if (equal(ch, arr[index++].code))
			fputc(arr[index - 1].ch, fp);
	}
	
	fclose(fp);
	fclose(file);
	return true;
}
void run() {
	while (true)
	{
		printf(" 1.压缩文件\n 2.解压文件\n 3.退出\n 请输入操作");
		char ch;
		scanf_s("%c", &ch,2);
		fflush(stdin);
		if (ch == '1')
		{
			printf("请输入目标文件路径：\n");
			char fileName[100];
			gets_s(fileName);
			if (compress(fileName))
			{
				printf("压缩完成");
				break;
			}
			else
			 printf("error,compress fail");
			break;
		}//输入 1
		if (ch == '2')
		{
			printf("请输入已压缩的文件地址：\n");
			char fileName[100];
			gets_s(fileName);
			if (uncompress(fileName))
			{
				printf("解压完成\n");
			}
			else
				printf("error,uncompress fail");
		} //输入 2 

		if (ch == '3')
			return;
	} // while 
}
		