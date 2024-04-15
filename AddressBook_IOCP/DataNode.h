#pragma once
#include <string>

class DataNode
{
	friend class UserList;
	DataNode* Next;

public:
	DataNode();
	virtual ~DataNode();
	DataNode* GetNext();
	virtual std::string Getkey()=0;
	virtual void PrintNode()=0;


};

