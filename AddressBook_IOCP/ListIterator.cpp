#include "ListIterator.h"

ListIterator::ListIterator()
	:Head(nullptr),Current(nullptr)
{

}

ListIterator::~ListIterator()
{
}

DataNode* ListIterator::GetCurrent() const
{
	return Current;
}

void ListIterator::MoveNext()
{
	if (Current != nullptr) {
		Current = Current->GetNext();
	}
}
