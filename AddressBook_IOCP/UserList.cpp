#include "UserList.h"
#include "UserData.h"
#include <iostream>

#include <Windows.h>

#include <sstream>

CRITICAL_SECTION cs;
static int ActiveWrite = 0;
 

UserList::UserList(DataNode* pHead)
{
	InitializeCriticalSection(&cs);
}

UserList::~UserList()
{
	DeleteCriticalSection(&cs);
	RemoveList();
	
}

 void UserList::FindNode(const std::string key)
{
	 //����� ������ ����
	 
	 //Ÿ�� ������ ���� Ȯ��
	 if (CoreVec.empty()) {
		 std::cout << "�� ����Ʈ �Դϴ�," << std::endl;
		
	 }
	 //CoreVec�� ���ٽ��� Ȱ���� find_if�� Ž���ϰ� Ž���� �ɶ����� result�� �߰�.
	 auto it = CoreVec.begin();

	 while (it != CoreVec.end()) {
		 it = std::find_if(it, CoreVec.end(), [&key](DataNode* node) { return node->Getkey() == key; });
		 if (it != CoreVec.end()) {
			 Tmp.push_back(*it);
			 it = CoreVec.erase(it); // erase ȣ�� �� �ݺ��� ������Ʈ
		 }

	 }
	 

	if (Tmp.empty()) {

		std::cout << "�ش� �̸��� �������� �ʽ��ϴ�." << std::endl;
	}
	else
		for (auto n : Tmp) {
			n->PrintNode();
		}
	
}

int UserList::AddVecNewNode(DataNode* NewNode) 
{
	//vector�� push_back
	CoreVec.push_back(NewNode);
	

	return CoreVec.size();
}

void UserList::PrintAll()
{
	for (auto n : CoreVec) {
		std::cout << static_cast<UserData*>(n)->GetN() << " "<<static_cast<UserData*>(n)->GetP() << std::endl;
	}


}

int UserList::RemoveNode(const std::string Name)
{

	
	

	
	return CoreVec.size();
}


void UserList::Clear()
{
	RemoveList();
}

void UserList::LoadFile()
{
	//���� ����
	HANDLE srcFile = CreateFile(
		L"C:\\Users\\zkdlx\\Desktop\\source\\repos\\AddressBook_IOCP\\Select_Folder\\Address_random.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
	if (srcFile == INVALID_HANDLE_VALUE) {
		std::cout << "���� ���¿� �����߽��ϴ�. ERROR NUM :" << errno << std::endl;
		return;
	}
	//zero - copy�� ���� ���� ���� ��ü
	LARGE_INTEGER FileSize;
	GetFileSizeEx(srcFile, &FileSize);
	HANDLE srcMap = CreateFileMapping(
		srcFile,
		nullptr, // ���ȼӼ��� ���
		PAGE_READONLY,//�б����� ���� ��ü
		0,				//���� 32��Ʈ
		FileSize.QuadPart,
		nullptr			//���� ��ü�� �̸����� �ٸ� ���μ��������� �� ���μ����� �̸����� �����ϴµ� �̴� IPC�� �����޸𸮱�� ����.
	);
	if (srcMap == NULL) {
		std::cout << "���� ��ü ������ ����" << std::endl;
		std::cout << "���� �ڵ� : " << GetLastError() << std::endl;
	}

	//LPVOID ���·� ��ȯ�ϴ� �Լ��� ����ȯ �ʿ�
	char* srcText = (char*)MapViewOfFile(
		srcMap,
		FILE_MAP_READ,
		0,				//����32 ����
		0,				//����32 ����
		0				//������ ������ ũ�� ����Ʈ ���� ���� , 0�̸� ���� ��ü
	);
	if (srcText == NULL) {
		std::cout << "" << std::endl;
		std::cout << "���� �ڵ� : " << GetLastError() << std::endl;
	}

	parseData(srcText);




	
	
	
	/* string�� �ѹ� ������ iss�� �Ľ��ؼ� ó��
	//���� ������ Ȯ��
	
	DWORD UnitByte = 22;
	DWORD TmpSize;
	DWORD Total = 0;

	std::string Tmp(UnitByte,'\0');

	
	std::cout << "������ ������ : " << FileSize.QuadPart << std::endl;

	while (true) {



		std::string Name, Phone;

		if (!ReadFile(srcFile, &Tmp[0], UnitByte, &TmpSize, nullptr)) {
			std::cout << "���� �б� ����" << std::endl;
			CloseHandle(srcFile);
			
			return;
		}
		
		std::string check=std::string(Tmp);
		std::istringstream iss(check);

		Total += TmpSize;

		iss >> Name >> Phone;

		

		AddNewNode(Name, Phone);
		if (Total == FileSize.QuadPart) {
			std::cout << "���� ó���� �������ϴ�." << std::endl;
			break;
		}

	}
	*/

	UnmapViewOfFile(srcText);
	CloseHandle(srcMap);
	CloseHandle(srcFile);
	
}

DWORD WINAPI WorkerThread(LPVOID Param)
{

	HANDLE hIocp = static_cast<HANDLE>(Param);
	DWORD bytesTransferred;
	ULONG_PTR completionKey;
	OVERLAPPED* pOverlapped;
	
	while (GetQueuedCompletionStatus(hIocp, &bytesTransferred, &completionKey, &pOverlapped, 10)) {
		if (completionKey != 1)
		{
			std::cout << "complete packet error" << std::endl;
		}
	}
	std::cout << "worker exit" << std::endl;
	return 0;
}

void CALLBACK FileWriteCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped)
{
	std::cout << "Write operation completed. Error code: " << dwErrorCode
		<< ", Number of bytes transferred: " << dwNumberOfBytesTransferred << std::endl;
	// Clean up the OVERLAPPED structure
	delete lpOverlapped;
}



void UserList::SaveFile()
{
	//���͸� ����
	std::wstring Route = L"C:\\Users\\zkdlx\\Desktop\\source\\repos\\AddressBook_IOCP\\Select_Folder\\";
	std::wstring File;
	std::cout << "������ ������ �̸��� �Է��ϼ���.(Ȯ���ڵ� �ʿ�)" << std::endl;
	std::wcin >> File;
	Route.append(File);


	//���� ���� ����
	HANDLE cFile = CreateFile(
		Route.c_str(),
		GENERIC_WRITE ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, // ���� �б� �� ���� ����
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // �񵿱� �÷���
		nullptr
	);

	if (cFile == INVALID_HANDLE_VALUE) {
		std::cout << "���� ������ �����߽��ϴ�." << std::endl;
		return;
	}

	// IOCP ��Ʈ ����
	HANDLE sIocp = CreateIoCompletionPort(cFile, nullptr, 1, 0);
	if (sIocp == nullptr) {
		std::cout << "IOCP ���� ���� " << GetLastError() << std::endl;
		CloseHandle(cFile);
		return;
	}

	// ��Ŀ ������ ����
	const int numberOfThreads = 3;  // ���÷� 2���� ������ ����
	std::vector<HANDLE> workerThreads(numberOfThreads);
	for (int i = 0; i < numberOfThreads; ++i) {
		workerThreads[i] = CreateThread(nullptr, 0, WorkerThread, sIocp, 0, nullptr);
		if (workerThreads[i] == nullptr) {
			std::cout << "Worker Thread ���� ���� " << GetLastError() << std::endl;
		}
	}
	std::vector<OVERLAPPED*> ol(CoreVec.size());
	std::vector<std::string> input(CoreVec.size());
	DWORD offset = 0;
	DWORD Trans = 0;
	for (size_t i = 0; i < CoreVec.size(); ++i) {
		input[i] = static_cast<UserData*>(CoreVec[i])->GetN() + " " + static_cast<UserData*>(CoreVec[i])->GetP() + '\n';
		
		ol[i] = new OVERLAPPED{};
		ZeroMemory(ol[i], sizeof(OVERLAPPED));
		ol[i]->Offset = offset;
		ol[i]->OffsetHigh = 0;
		offset += input[i].length();

		BOOL result = WriteFile(
			cFile,
			input[i].data(),
			input[i].length(),
			&Trans,
			ol[i]
		);
		if (GetLastError() != ERROR_IO_PENDING)
			std::cout << "�۾� ���� ����" << std::endl;
		/*
		if (!result && GetLastError() != ERROR_IO_PENDING) {
			std::cout << "���⿡ �����߽��ϴ�. ���� �ڵ�: " << GetLastError() << std::endl;
			break;
		}
		*/
	}
//	SleepEx(INFINITE, TRUE);
//	PostQueuedCompletionStatus(sIocp, 0, 1, NULL); // ���� ��ȣ

	// ��Ŀ ������ ���� ���
	WaitForMultipleObjects(workerThreads.size(), workerThreads.data(), TRUE, INFINITE);
	for (HANDLE thread : workerThreads) {
		CloseHandle(thread);
	}

	CloseHandle(sIocp);
	CloseHandle(cFile);
	
	std::cout << "Save�Լ� ����" << std::endl;
}







void UserList::RemoveList()
{

}

void UserList::parseData(const char* data)
{
	std::string name, phone; // �̸��� ��ȭ��ȣ�� ������ ����
	const char* tokenStart = data; // ���� ��ū(�ʵ�)�� ����
	const char* p = data; // �����͸� ��ȸ�ϴ� ������

	while (*p) { // �� ���ڸ� ���� ������ �ݺ�
		if (*p == ' ') { // �ʵ� �����ڸ� ������ (�̸��� ��ȭ��ȣ ����)
			name.assign(tokenStart, p - tokenStart); // �̸� ����
			tokenStart = p + 1; // ��ū ������ ���� ��ġ�� ������Ʈ
		}
		else if (*p == '\n') { // ���ڵ� �����ڸ� ������ (�� ������ ��)
			phone.assign(tokenStart, p - tokenStart); // ��ȭ��ȣ ����
			AddVecNewNode(new UserData(name,phone)); // ������ �̸��� ��ȭ��ȣ�� �� ��� �߰�

			// ���� ���ڵ� ó���� ���� ���� �ʱ�ȭ
			name = "";
			phone = "";
			tokenStart = p + 1;
		}
		++p; // ������ �̵�
	}

	// ���� ���� �������� �� ������ ���ڵ� ó��
	if (p != tokenStart) {
		phone.assign(tokenStart, p - tokenStart); // ������ ��ȭ��ȣ ����
		AddVecNewNode(new UserData(name,phone)); // ������ ���ڵ忡 ���� ó��
	}
}
