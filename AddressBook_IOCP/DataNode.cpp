#include "DataNode.h"

DataNode::DataNode()
	:Next(nullptr)
{
}

DataNode::~DataNode()
{
}

DataNode* DataNode::GetNext()
{
	return Next;
}
