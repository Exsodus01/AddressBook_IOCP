#include "UserInterface.h"
#include "UserData.h"
#include <iostream>
#include <chrono>
#include "ListIterator.h"
#include "UserList.h"
#include <unordered_map>
#include <Windows.h>
#include <thread>

UserInterface::UserInterface(UserList& rList) //참조는 생성자 초기화 목록으로 초기화
	:List(rList)
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::Add()
{
	std::cout << '\n' << '\n';
	std::string str, Pho;
	std::cout << "입력한 이름과 번호를 [이름 번호(0000-0000-0000)] 형식(대괄호 제외)으로 입력하세요." << std::endl;
	std::cin >> str >> Pho;
	auto start = std::chrono::high_resolution_clock::now();
	List.AddVecNewNode(new UserData(str,Pho));
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;

	//std::cout << "입력이 완료되었습니다." << " 남은 노드 개수 : " << UserData::GetUserDataCount() << std::endl;
}

void UserInterface::Search()
{
	std::cout << '\n' << '\n';
	std::string str;
	std::cout << "찾을 이름을 입력하세요." << std::endl;
	std::cin >> str;
	auto start = std::chrono::high_resolution_clock::now();
	List.FindNode(str);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;

}

void UserInterface::RemoveN()
{
	std::cout << '\n' << '\n';
	std::string str;
	std::cout << "삭제할 이름을 입력하세요." << std::endl;
	std::cin >> str;

	auto start = std::chrono::high_resolution_clock::now();
	List.RemoveNode(str);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;
	//std::cout << "삭제가 완료되었습니다." << "남은 노드 개수 : " << UserList::CoreVec.size() << std::endl;
}

int UserInterface::PrintUI()
{
	std::cout << '\n' << '\n';
	int tmp = 0;
	std::cout <<"0.exit" << "\n" << "1.Add" << "\n" << "2.Search" << "\n" << "3.Remove 노드" << "\n" << "4.Remove 리스트" << "\n" << "5.Print All" <<"\n"<<"6.LoadFile" <<"\n"<<"7.SaveFile"<< std::endl;
	std::cout << "필요한 번호를 입력하세요" << std::endl;

	std::cin >> tmp;
	return tmp;
}

int UserInterface::Run()
{
	int Menu = 0;
	while ((Menu = PrintUI()) != 0) {
		switch (Menu) {

		case 1:
			Add();
			break;

		case 2:
			Search();
			break;

		case 3:
			RemoveN();
			break;

		case 4:
			RemoveL();
			break;

		case 5:
			PrintList();
			break;

		case 6:
			LoadF();
			break;

		case 7:
			SaveF();
			break;
		case 8:
		
			std::cout << "원하는 스레드의 개수를 입력하세요." << std::endl;

			break;
		default :
			std::cout << "잘못된 값을 입력했습니다. 다시 입력하세요." << std::endl;

		}
	}




	return 0;
}

void UserInterface::PrintList()
{
	std::cout << '\n' << '\n';

	
	
	List.PrintAll();


	//std::cout << "전체 노드의 개수 : " << UserData::GetUserDataCount() << std::endl;
}

void UserInterface::RemoveL()
{

	std::cout << '\n' << '\n';
	auto start = std::chrono::high_resolution_clock::now();
	List.Clear();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;

	//std::cout << "리스트가 완전히 삭제 되었습니다." << " 남은 노드 개수 : " << UserData::GetUserDataCount() << std::endl;

}

void UserInterface::LoadF()
{
	std::cout << '\n' << '\n';
	auto start = std::chrono::high_resolution_clock::now();
	List.LoadFile();
	auto end = std::chrono::high_resolution_clock::now(); 
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;
}

void UserInterface::SaveF()
{
	std::cout << '\n' << '\n';
	auto start = std::chrono::high_resolution_clock::now();
	List.SaveFile();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;
}





