#pragma once



class UserList;
class UserInterface
{
public:
	UserInterface(UserList& rList);
	~UserInterface();


	void Add();

	void Search();

	void RemoveN();

	int PrintUI();

	int Run();

	void PrintList();

	void RemoveL();

	void LoadF();
	
	void SaveF();

	

	

	

protected:
	UserList& List;

};

