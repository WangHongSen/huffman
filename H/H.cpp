// H.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SCALE 128
#define CODELEN 10

template <typename E, typename Comp>
class Heap {
private:
	E* heap;
	int curSize;
	int maxSize;
	//定义一个辅助函数使得元素位于正确的位置 
	void shiftDown(int pos) {
		while (!isLeaf(pos)) {
			int small = leftChild(pos); //假设左子节点值较小
			int rc = rightChild(pos);
			if ((rc < curSize)&&compare(rc,small))
				small = rc;
			if (compare(small,pos))
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
		//if (curSize == 1 && pos == 0)
		//	return  false;//当只有一个节点是，认定他不是叶节点
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
//将node数组初始化成所有节点的frequency为零，
//长度初始化为CODELEN,10，code字符串
//初始化为-,最后加上\0,ch字符初始化为对应下标字符
void initialNode(node* arr)
{
	for (unsigned int i = 0; i < SCALE; i++)
	{
		arr[i].code = new char[CODELEN + 1];
		for (int j = 0; j < CODELEN - 1; j++)
			arr[i].code[j] = '-';
		arr[i].code[CODELEN] = '\0';
		arr[i].frequency = 0;
		arr[i].ch = i;
	}
}

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
		return lc;
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
		return one->getWeight() < two->getWeight();
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
bool compress(char* fileName);
void encode(node* arr, char* content, HuffmanNode* root, int level );
HuffmanNode* readConfigure(FILE* file);
bool uncompress(char* fileName);
HuffTree*  generateCode(node* arr, FILE* file, HuffTree* tree);
int stringToNumber(char* str) {
	int n = 0;
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '1')
			n += pow(2, len-1-i);

	}
	return n;
}
int main()
{
	  run();
	//下面是一些测试用例
	//char s[100] = "d:\\a.txt";
   // compress(s);
	//FILE* fp;
	/**fopen_s(&fp, "d:\\2017秋\\数据结构\\project2\\a.txt", "r");
	node arr[SCALE];
	for (unsigned int i = 0; i < SCALE; i++)
	{
		arr[i].ch = (char)i;
		arr[i].frequency = 0;
		arr[i].code = new char[CODELEN + 1];
		arr[i].code[CODELEN] = '\0';
	}
	HuffTree* tree;
	tree = new HuffTree();
	generateCode(arr, fp, tree);
	printf("ok\n");*/
	//char addr[100] = "d:\\2017秋\\数据结构\\project2\\a.txt";
	//compress(addr);
/**	leaf a('a', 3);
	leaf s('s', 2);
	leaf d('d', 1);
	intNode e(&d,&s);
	intNode f(&a, &e);
	node arr[3];
	arr[0].ch = 'a';
	arr[0].frequency = 3;
	arr[0].code = new char[10];
	arr[1].ch = 's';
	arr[1].frequency = 2;
	arr[1].code = new char[10];
	arr[2].ch = 'd';
	arr[2].frequency = 1;
	arr[2].code = new char[10];
	char str[4];
	encode(arr, str, &f,0);
	for (int i = 0; i < 3; i++)
		printf("%c has code %s\n",arr[i].ch,arr[i].code);*/
	/**char a1[100] = "D:\\practice\\huffman\\H\\H\\congig.txt";
	FILE* fp;
	fopen_s(&fp, a1, "r");
	intNode* tree;
	tree = new intNode();
	tree =(intNode*) readConfigure(fp);
	if (tree != NULL)
		printf("OK");*/
	//char ad2[100] = "d:\\2017秋\\数据结构\\project2\\a.huf";
	//uncompress(ad2);
}



void encode(node* arr,char* content,HuffmanNode* root,int level)
{
	if (root->isLeaf())
	{
		char item = ((leaf*)root)->value();
		int i = 0;
		while (arr[i++].ch != item);
		strcpy_s(arr[i-1].code,strlen(content)+1,content);
	//	printf("%c has code is %s\n", item,arr[i-1].code);
	}
	else {
		content[level] = '0';
		content[level + 1] = '\0';
		encode(arr, content, ((intNode*)root)->getLeftChild(), level+1);
		content[level] = '1';
		//content[level + 1] = '\0';
		encode(arr, content, ((intNode*)root)->getRightChild(), level + 1);
	}
}
//在arr中写入文件中各个字符的出现频率
//返回记录的字符类型数量
int read(FILE* file, node* arr)
{
	
	int std[SCALE] = { 0 };
	int ch;
	int count= 0;
	//统计文件中各个字符的出现频率
	while ((ch = fgetc(file)) != EOF)
	{
		std[(int)ch]++;
		//printf("%c", ch);
	}
	
	for (int i = 0; i < SCALE; i++)
	{
		if (std[i] != 0)
		{
			arr[i].frequency = std[i];
			arr[i].ch = (char)i;
			count++;
		}
	}
	return count;
}
HuffTree* generateCode(node* arr,FILE* file, HuffTree* tree)
{
	int order = read(file, arr);
  //构造相应的hufffmantree
	HuffTree* forest[SCALE];
	//初始化forest
	int j = 0;
	for (int i = 0; i < SCALE; i++)
	{
		if (arr[i].frequency != 0)
		{
			forest[j++] = new HuffTree(arr[i].ch, arr[i].frequency);
		}
	}
	tree = buildHuffTree(forest,order);
	char content[CODELEN*2];//一定得超出CODELEN不然会产生奇怪的BUG
	encode(arr, content, tree->getRoot(), 0);
	//补足八位编码
	/**
	for (int i = 0; i < SCALE; i++)
	{
		if (arr[i].frequency != 0)
		{
			int len = strlen(arr[i].code);
			if (len < 8)
				for (int j = 7; j >= len; j--)
					arr[i].code[j] = '0';
			arr[i].code[8] = '\0';
			//printf("%c has code %s\n", arr[i].ch, arr[i].code);
		}
		
	}*/
	return tree;
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
		writeConfigure(file, ((intNode*)root)->getLeftChild());
		writeConfigure(file,((intNode*)root)->getRightChild());
		
	}
}

void write(FILE* result, FILE* reader, node* arr)
{
	char ch ;	
	
	//写入编码后的转换文本
	fseek(reader, 0, SEEK_SET);
	while ((ch = fgetc(reader)) != EOF)
	{
		//printf("%c\n", ch);
		int index = 0;
		//查找对应编码
		while (index<SCALE)
		{
			if (ch == (char)arr[index].ch)
			{
				//printf("write arr.ch %c code %s to char %c\n", arr[index].ch, arr[index].code, ch);
				int temp = stringToNumber(arr[index].code);
				fwrite(&temp, sizeof(int), 1,result);
				//fputs(arr[index].code, result);
				break;
			}
			else
				index++;
		}
		//写入编码
		
	}
	 //loop 写入编码文本
}
bool compress(char* fileName)
{
	FILE* reader;
	fopen_s(&reader, fileName, "r+");
	node arr[SCALE];
	//初始化arr
	initialNode(arr);

	HuffTree* tree;
	tree = new HuffTree();
	tree = generateCode(arr, reader,tree);//生成编码
	
	FILE* conf;
	fopen_s(&conf,"configure.txt", "a+"); //创建配置文件
	//写入配置文件
	writeConfigure(conf, tree->getRoot());
	fclose(conf);
	
	
	int lof = strlen(fileName) - 4; //计算文件后缀名的开始下标
	strcpy_s(fileName + lof, 5, ".huf");
	FILE* result;
	fopen_s(&result, fileName, "a+");
	fseek(reader, 0, SEEK_SET);//将文件指针重新指向头部
	write(result, reader, arr);
	//printf(" write succesfully\n");

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

//
int decode(node* arr, char* content, HuffmanNode* root, int level)
{
	static int count = 0; //记录生成了多少个节点
	if (root->isLeaf())
	{
		char item = ((leaf*)root)->value();
		count++;
		arr[item].frequency = 1;
		strcpy_s(arr[item].code, strlen(content) + 1, content);
		//	printf("%c has code is %s\n", item,arr[i-1].code);
	}
	else {
		content[level] = '0';
		content[level + 1] = '\0';
		decode(arr, content, ((intNode*)root)->getLeftChild(), level + 1);
		content[level] = '1';
		//content[level + 1] = '\0';
		decode(arr, content, ((intNode*)root)->getRightChild(), level + 1);
	}
	return count;
}
bool uncompress(  char* fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "r+");
	if (file == NULL)
	{
		printf("error,file does not exit");
		return false;
	}

	node arr[SCALE];
	initialNode(arr);
	HuffmanNode* tree;
	//读取配置，生成编码树
	FILE* conf;
     fopen_s(&conf, "configure.txt", "r");
	tree = readConfigure(conf);
	
	fclose(conf);
	//产生编码
	char content[15];
	int count = decode(arr, content, tree, 0);
	//补足八位编码
	for (int i = 0; i < SCALE; i++)
	{
		if (arr[i].frequency != 0)
		{
			int len = strlen(arr[i].code);
			if (len < 8)
				for (int j = 7; j >= len; j--)
					arr[i].code[j] = '0';
			arr[i].code[8] = '\0';
		}
		
		//printf("char %c has code %s\n", arr[i].ch, arr[i].code);
	} 
	//开始译码，创建一个新的文件并写入
	FILE* fp;
	int lof = strlen(fileName) - 4; //计算文件后缀名的开始下标
	strcpy_s(fileName + lof, 5, ".txt");
	fopen_s(&fp,fileName, "a");
	char ch[10];
	int len = sizeof(char)*8 +1;
	fseek(file, 0, SEEK_SET);
	fgets(ch,len,file);
	int index;
	while (!feof(file))
	{
		index = 0;
		while (index < SCALE)
		{
			if ( arr[index].frequency!=0 && equal(ch, arr[index].code))
			{
				fputc(arr[index].ch, fp);
				printf("put %c arr %s   ch %s\n", arr[index].ch,arr[index].code, ch);
				break;
			}
			index++;
	}//while 查找对应node
		fgets(ch, len, file);
	}
	
	fclose(fp);
	fclose(file);
	return true;
}
void run() {
	char ch;
	while (true)
	{
		printf(" 1.压缩文件\n 2.解压文件\n 3.退出\n 请输入操作:\n");
		std::cin >> ch;
		std::cin.clear();
		std::cin.sync();
	//	scanf_s("%c", &ch,2);
		//fflush(stdin);
		if (ch == '1')
		{
			printf("请输入目标文件路径：\n");
			char fileName[100];
			std::cin >> fileName;
			std::cin.clear();
			std::cin.sync();
			//printf("You input %s\n", fileName);
			//gets_s(fileName);
			if (compress(fileName))
			{
				printf("压缩完成\n");
				continue;
			}
			else
			{
				printf("error,compress fail\n");
			}
		
		}//输入 1
		if (ch == '2')
		{
			printf("请输入已压缩的文件地址：\n");
			char fileName[100];
			std::cin >> fileName;
		//	gets_s(fileName);
			std::cin.clear();
			std::cin.sync();
			if (uncompress(fileName))
			{
				printf("解压完成\n");
				continue;
			}
			else
			{
				printf("error,uncompress fail");
				continue;
			}
		} //输入 2 

		if (ch == '3')
			return;
	} // while 
}
		