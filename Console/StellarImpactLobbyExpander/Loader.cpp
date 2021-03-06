#include "Loader.h"

std::string GameName = "Stellar Impact";

DWORD LobbyBaseAddress = {0x00BBC7CC}; //Baseaddress 

DWORD LobbyOffsets[] = { 0x0, 0x8, 0x14, 0x2C, 0x70 };

unsigned char bytes[4];

LPCSTR LGameWindow = "Stellar Impact"; 

std::string WelcommeText = "Type in your lobby size: ";

std::string GameStatus;

bool IsGameAvail;

Loader::Loader(void)
{
}

void Loader::Load(void){
	SetConsoleTitle("Stellar Impact Lobby Expander");
	HWND hGameWindow = NULL;
	int GameAvailTMR = clock();
	DWORD dwProcId = NULL;
	HANDLE hProcHandle = NULL;
	while(!GetAsyncKeyState(VK_ESCAPE))
	{	
		if(clock() - GameAvailTMR > 400)
		{
			system("cls");
			std::cout << "###########################################" << std::endl;
			std::cout << "#       Stellar Impact Lobby Expander     #" << std::endl;
			std::cout << "###########################################" << std::endl;
			GameAvailTMR = clock();
			IsGameAvail = false;
			hGameWindow = FindWindow( NULL, LGameWindow);
			if(hGameWindow)
			{
				
				GetWindowThreadProcessId( hGameWindow, &dwProcId ); 
				if( dwProcId != 0 ) 
				{ 
				hProcHandle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcId ); 
				if( hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL ) 
				{ 
					GameStatus = "Failed to open process for valid handle"; 
				} 
				else
				{
					GameStatus = GameName + " found";
					IsGameAvail = true;
				}
				} 
				else GameStatus = "Failed to obtain process id";
			}
			else
			{
				GameStatus = GameName + " not found";
			}
			std::cout <<GameStatus<< std::endl;
			if(IsGameAvail)
			{
				std::cout <<WelcommeText;
				std::string  text;
				std::cin >> text;
				int value = atoi(text.c_str());
				ValueToByte(value);
				if(IsGameAvail)
				{
					WriteToMemory(hProcHandle);
				}
			}
			
		}
			
	}
	try{
		CloseHandle( hProcHandle ); 
		CloseHandle(hGameWindow);
	}
	catch(EXCEPINFO x)
	{
	}
}

Loader::~Loader(void)
{
}


DWORD Loader::FindDmaAddress(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;  

	DWORD pTemp;

	DWORD pointerAddr;
	for(int i = 0; i < PointerLevel; i ++)
	{
		if(i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
		}

		pointerAddr = pTemp + Offsets[i];   
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);
	}
	return pointerAddr;
}


void Loader::WriteToMemory(HANDLE hProcHandle)
{
	DWORD LobbyAddressToWrite = FindDmaAddress(5, hProcHandle, LobbyOffsets, LobbyBaseAddress);
	BYTE val[4]; 
	CharToByte((char*)bytes, (BYTE*)val,4);
	WriteProcessMemory( hProcHandle, (BYTE*)LobbyAddressToWrite, &val, sizeof(val), NULL);
}

void Loader::ValueToByte(int val)
{
	bytes[3] = (val >> 24) & 0xFF;
	bytes[2] = (val >> 16) & 0xFF;
	bytes[1] = (val >> 8) & 0xFF;
	bytes[0] = val & 0xFF;
}

void Loader::CharToByte(char* chars, byte* bytes, unsigned int count)
{
    for(unsigned int i = 0; i < count; i++)
    	bytes[i] = (byte)chars[i];
}