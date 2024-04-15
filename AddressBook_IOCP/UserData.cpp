#include "UserData.h"
#include <iostream>


UserData::UserData()
	:UserName(""), UserPhone("")
{
	
}

UserData::UserData(std::string Name, std::string Phone)
{
	UserName = Name;
	UserPhone = Phone;
}

const std::string UserData::GetN()
{
	return UserName;
}

UserData::~UserData()
{
}

const std::string UserData::GetP()
{
	return UserPhone;
}





std::string UserData::Getkey()
{
	return UserName;
}

void UserData::PrintNode()
{
	std::cout << UserName << " " << UserPhone << std::endl;
}
