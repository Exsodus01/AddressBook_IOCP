#pragma once
#include "DataNode.h"
#include <vector>
#include <list>
#include <windows.h>
#include "ListIterator.h"

class UserList
{
public:
	UserList(DataNode* pHead);
	~UserList();


	

	void FindNode(const std::string key);			

	int AddVecNewNode(DataNode* NewNode);


	int RemoveNode(const std::string key);

	void Clear();

	void PrintAll();
	void LoadFile();

	void SaveFile();


protected:
    void RemoveList();
	void parseData(const char* data);
	

	std::vector<DataNode*> CoreVec;
	std::list<DataNode*> CoreList;

	std::vector<DataNode*> Tmp;

	



};

