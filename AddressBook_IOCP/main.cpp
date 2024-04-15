#include <iostream>

#include <vector>

#include "UserInterface.h"
#include "UserList.h"
#include "UserData.h"


int main() {



	UserList DB(new UserData);
	UserInterface UI(DB);
	UI.Run();
   
	



	return 0;
}