// H.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SCALE 128

int* count(int* sta, std::string line) {
	int len = line.length();
	for (int i = 0; i < len; i++)
	{
		char ch = line.at(i);
		sta[ch] ++;
	}

	return sta;
}

//传入含有各个字符出现频率的数组，一个node数组
//返回ndoe的大小
int makeNode(int* sta, node* arr)
{
	int order = 0;
	for (int i = 0; i < SCALE; i++)
	{
		if (sta[i] != 0)
		{
			node curr;
			curr.ch = i;
			curr.frequency = sta[i];
			arr[order++] = curr;

		}
	}

	return order;
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

int main()
{
	std::string str = "assdddfrrasddgfggfere";
	int times[SCALE] = { 0 };
	node arr[SCALE];
	count(times, str);
	display(times);
	int len =makeNode(times, arr);
	heap one(arr,len,SCALE);
	one.printHeap();
}

