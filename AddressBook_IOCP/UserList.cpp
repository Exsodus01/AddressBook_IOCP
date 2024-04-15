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
	 //결과를 저장할 벡터
	 
	 //타겟 백터의 상태 확인
	 if (CoreVec.empty()) {
		 std::cout << "빈 리스트 입니다," << std::endl;
		
	 }
	 //CoreVec를 람다식을 활용한 find_if로 탐색하고 탐색이 될때마다 result에 추가.
	 auto it = CoreVec.begin();

	 while (it != CoreVec.end()) {
		 it = std::find_if(it, CoreVec.end(), [&key](DataNode* node) { return node->Getkey() == key; });
		 if (it != CoreVec.end()) {
			 Tmp.push_back(*it);
			 it = CoreVec.erase(it); // erase 호출 후 반복자 업데이트
		 }

	 }
	 

	if (Tmp.empty()) {

		std::cout << "해당 이름이 존재하지 않습니다." << std::endl;
	}
	else
		for (auto n : Tmp) {
			n->PrintNode();
		}
	
}

int UserList::AddVecNewNode(DataNode* NewNode) 
{
	//vector에 push_back
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
	//파일 열기
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
		std::cout << "파일 오픈에 실패했습니다. ERROR NUM :" << errno << std::endl;
		return;
	}
	//zero - copy를 위한 파일 매핑 객체
	LARGE_INTEGER FileSize;
	GetFileSizeEx(srcFile, &FileSize);
	HANDLE srcMap = CreateFileMapping(
		srcFile,
		nullptr, // 보안속성은 상속
		PAGE_READONLY,//읽기전용 매핑 객체
		0,				//상위 32비트
		FileSize.QuadPart,
		nullptr			//매핑 객체의 이름으로 다른 프로세스에서는 이 프로세스에 이름으로 접근하는데 이는 IPC의 공유메모리기법 때문.
	);
	if (srcMap == NULL) {
		std::cout << "매핑 객체 생성에 실패" << std::endl;
		std::cout << "에러 코드 : " << GetLastError() << std::endl;
	}

	//LPVOID 형태로 반환하는 함수라서 형변환 필요
	char* srcText = (char*)MapViewOfFile(
		srcMap,
		FILE_MAP_READ,
		0,				//상위32 시작
		0,				//하위32 시작
		0				//매핑할 데이터 크기 바이트 단위 지정 , 0이면 파일 전체
	);
	if (srcText == NULL) {
		std::cout << "" << std::endl;
		std::cout << "에러 코드 : " << GetLastError() << std::endl;
	}

	parseData(srcText);




	
	
	
	/* string에 한번 복사후 iss로 파싱해서 처리
	//파일 사이즈 확인
	
	DWORD UnitByte = 22;
	DWORD TmpSize;
	DWORD Total = 0;

	std::string Tmp(UnitByte,'\0');

	
	std::cout << "파일의 사이즈 : " << FileSize.QuadPart << std::endl;

	while (true) {



		std::string Name, Phone;

		if (!ReadFile(srcFile, &Tmp[0], UnitByte, &TmpSize, nullptr)) {
			std::cout << "파일 읽기 실패" << std::endl;
			CloseHandle(srcFile);
			
			return;
		}
		
		std::string check=std::string(Tmp);
		std::istringstream iss(check);

		Total += TmpSize;

		iss >> Name >> Phone;

		

		AddNewNode(Name, Phone);
		if (Total == FileSize.QuadPart) {
			std::cout << "파일 처리가 끝났습니다." << std::endl;
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
	//디렉터리 설정
	std::wstring Route = L"C:\\Users\\zkdlx\\Desktop\\source\\repos\\AddressBook_IOCP\\Select_Folder\\";
	std::wstring File;
	std::cout << "생성할 파일의 이름을 입력하세요.(확장자도 필요)" << std::endl;
	std::wcin >> File;
	Route.append(File);


	//저장 파일 생성
	HANDLE cFile = CreateFile(
		Route.c_str(),
		GENERIC_WRITE ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, // 동시 읽기 및 쓰기 공유
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // 비동기 플래그
		nullptr
	);

	if (cFile == INVALID_HANDLE_VALUE) {
		std::cout << "파일 생성에 실패했습니다." << std::endl;
		return;
	}

	// IOCP 포트 생성
	HANDLE sIocp = CreateIoCompletionPort(cFile, nullptr, 1, 0);
	if (sIocp == nullptr) {
		std::cout << "IOCP 생성 실패 " << GetLastError() << std::endl;
		CloseHandle(cFile);
		return;
	}

	// 워커 스레드 생성
	const int numberOfThreads = 3;  // 예시로 2개의 스레드 생성
	std::vector<HANDLE> workerThreads(numberOfThreads);
	for (int i = 0; i < numberOfThreads; ++i) {
		workerThreads[i] = CreateThread(nullptr, 0, WorkerThread, sIocp, 0, nullptr);
		if (workerThreads[i] == nullptr) {
			std::cout << "Worker Thread 생성 실패 " << GetLastError() << std::endl;
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
			std::cout << "작업 예약 실패" << std::endl;
		/*
		if (!result && GetLastError() != ERROR_IO_PENDING) {
			std::cout << "쓰기에 실패했습니다. 에러 코드: " << GetLastError() << std::endl;
			break;
		}
		*/
	}
//	SleepEx(INFINITE, TRUE);
//	PostQueuedCompletionStatus(sIocp, 0, 1, NULL); // 종료 신호

	// 워커 스레드 종료 대기
	WaitForMultipleObjects(workerThreads.size(), workerThreads.data(), TRUE, INFINITE);
	for (HANDLE thread : workerThreads) {
		CloseHandle(thread);
	}

	CloseHandle(sIocp);
	CloseHandle(cFile);
	
	std::cout << "Save함수 종료" << std::endl;
}







void UserList::RemoveList()
{

}

void UserList::parseData(const char* data)
{
	std::string name, phone; // 이름과 전화번호를 저장할 변수
	const char* tokenStart = data; // 현재 토큰(필드)의 시작
	const char* p = data; // 데이터를 순회하는 포인터

	while (*p) { // 널 문자를 만날 때까지 반복
		if (*p == ' ') { // 필드 구분자를 만나면 (이름과 전화번호 사이)
			name.assign(tokenStart, p - tokenStart); // 이름 추출
			tokenStart = p + 1; // 토큰 시작을 다음 위치로 업데이트
		}
		else if (*p == '\n') { // 레코드 구분자를 만나면 (각 라인의 끝)
			phone.assign(tokenStart, p - tokenStart); // 전화번호 추출
			AddVecNewNode(new UserData(name,phone)); // 추출한 이름과 전화번호로 새 노드 추가

			// 다음 레코드 처리를 위해 변수 초기화
			name = "";
			phone = "";
			tokenStart = p + 1;
		}
		++p; // 포인터 이동
	}

	// 파일 끝에 도달했을 때 마지막 레코드 처리
	if (p != tokenStart) {
		phone.assign(tokenStart, p - tokenStart); // 마지막 전화번호 추출
		AddVecNewNode(new UserData(name,phone)); // 마지막 레코드에 대한 처리
	}
}
