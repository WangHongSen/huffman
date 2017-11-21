#pragma once
#include<cstdio>
#include<cstdlib>
struct node {
	char ch;
	int frequency;
};
class heap {
private:
	node* Heap;
	int maxSize;
	int currSize;
	void shiftdown(int pos) 
	{
		while (!isLeaf(pos))
		{
			int small = left(pos); //假定左子节点较小
			int rc = right(pos);
			//右子节点比左子节点小，交换二者，使small变成rc
			if (rc < currSize && compare(small,rc))
				small  = rc;
			//因为pos不是叶子节点，所有一定有至少一个子节点，故不用判断
			//big是不是在currSize内
			if (compare(small,pos))
				return;//small代表节点frequency 小于父节点，直接返回
			swap(small,pos);
			pos = small;
		}
	}
	//如果former代表节点frequency大于late的，返回true
	inline bool compare(int fomer, int late)
	{
		return Heap[fomer].frequency > Heap[late].frequency;
	 }
	void swap(int fomer, int late)
	{
		node cur = Heap[fomer];
		Heap[fomer] = Heap[late];
		Heap[late] = cur;
	}
public:
	heap(node* arr, int curr, int max)
	{
		Heap = arr;
		currSize = curr;
		maxSize = max;
		buildHeap();
	}
	int size()
	{
		return currSize;
	}
	bool isLeaf(int pos)
	{
		return (pos >= (currSize / 2)) && (pos < currSize);
			
	}
	int left(int pos) const
	{
		return 2 * pos + 1;
	}
	int right(int pos) const
	{
		return 2 * pos + 1;
	}
	int parent(int pos) const
	{
		return (pos - 1) / 2;
	}

	void buildHeap()
	{
		for (int i = 0; i < currSize / 2 - 1; i++)
		{
			shiftdown(i);
		}
	}

	void insert(const node& item)
	{
		if (currSize >= maxSize)
		{
			printf("insert error, heap is fill");
			exit(0);
		}
		int curr = currSize++;
		Heap[curr] = item;
		while ((curr != 0) && (compare(parent(curr), curr)))
		{
			swap(parent(curr), curr);
			curr = parent(curr);
		}
	}
	//删除frequency最小的节点
	node removeFirst()
	{
		if (currSize <0)
		{
			printf("remove error,heap is empty");
			exit(0);
		}
		//交换第一个节点（最小节点）与最后一个节点，在把第一个节点往下拉
		swap(0, currSize--);
		if (currSize != 0)
		{
			shiftdown(0);
		}

		return Heap[currSize];
	}

	node remove(int pos)
	{
		if (pos >= currSize || pos < 0)
		{
			printf("wrong position;");
			exit(0);
		}
		if (pos == currSize-1)
		{
			//pos 是最后一个元素
			currSize--;
		}
		else {
			swap(pos, --currSize);
			while ((pos != 0) && compare(parent(pos), pos))
			{
				swap(parent(pos), pos);
				pos = parent(pos);
			}
		}

		return Heap[currSize];
	}

	void printHeap()
	{
		for (int i = 0; i < currSize; i++)
		{
			printf("char %c occurs %d times\n", Heap[i].ch, Heap[i].frequency);
		}
	}
};