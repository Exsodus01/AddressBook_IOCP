#pragma once
#include "DataNode.h"


class ListIterator
{
	friend class UserList;

private:
	DataNode* Head;
	DataNode* Current;


public:
	ListIterator();
	~ListIterator();

	DataNode* GetCurrent() const;
	void MoveNext();


};

