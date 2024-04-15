#include "UserInterface.h"
#include "UserData.h"
#include <iostream>
#include <chrono>
#include "ListIterator.h"
#include "UserList.h"
#include <unordered_map>
#include <Windows.h>
#include <thread>

UserInterface::UserInterface(UserList& rList) //������ ������ �ʱ�ȭ ������� �ʱ�ȭ
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
	std::cout << "�Է��� �̸��� ��ȣ�� [�̸� ��ȣ(0000-0000-0000)] ����(���ȣ ����)���� �Է��ϼ���." << std::endl;
	std::cin >> str >> Pho;
	auto start = std::chrono::high_resolution_clock::now();
	List.AddVecNewNode(new UserData(str,Pho));
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;

	//std::cout << "�Է��� �Ϸ�Ǿ����ϴ�." << " ���� ��� ���� : " << UserData::GetUserDataCount() << std::endl;
}

void UserInterface::Search()
{
	std::cout << '\n' << '\n';
	std::string str;
	std::cout << "ã�� �̸��� �Է��ϼ���." << std::endl;
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
	std::cout << "������ �̸��� �Է��ϼ���." << std::endl;
	std::cin >> str;

	auto start = std::chrono::high_resolution_clock::now();
	List.RemoveNode(str);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;
	//std::cout << "������ �Ϸ�Ǿ����ϴ�." << "���� ��� ���� : " << UserList::CoreVec.size() << std::endl;
}

int UserInterface::PrintUI()
{
	std::cout << '\n' << '\n';
	int tmp = 0;
	std::cout <<"0.exit" << "\n" << "1.Add" << "\n" << "2.Search" << "\n" << "3.Remove ���" << "\n" << "4.Remove ����Ʈ" << "\n" << "5.Print All" <<"\n"<<"6.LoadFile" <<"\n"<<"7.SaveFile"<< std::endl;
	std::cout << "�ʿ��� ��ȣ�� �Է��ϼ���" << std::endl;

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
		
			std::cout << "���ϴ� �������� ������ �Է��ϼ���." << std::endl;

			break;
		default :
			std::cout << "�߸��� ���� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << std::endl;

		}
	}




	return 0;
}

void UserInterface::PrintList()
{
	std::cout << '\n' << '\n';

	
	
	List.PrintAll();


	//std::cout << "��ü ����� ���� : " << UserData::GetUserDataCount() << std::endl;
}

void UserInterface::RemoveL()
{

	std::cout << '\n' << '\n';
	auto start = std::chrono::high_resolution_clock::now();
	List.Clear();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 

	std::cout << "Function took " << duration << " milliseconds to complete." << std::endl;

	//std::cout << "����Ʈ�� ������ ���� �Ǿ����ϴ�." << " ���� ��� ���� : " << UserData::GetUserDataCount() << std::endl;

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





