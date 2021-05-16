#pragma once

#ifndef _KINTERFACE_H_
#define _KINTERFACE_H_

#pragma warning (disable: 4302 4311 4244 4700 )

#include <Windows.h>
#include <apiquery2.h>
#include <string>
#include "Utils.h"

#define IO_GET_ID_REQUEST  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6210, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6211, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6212, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IO_GET_MODULE_REQUEST  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6213, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST
{
	ULONG ProcessId; //target process id
	ULONG Address; // address of memory to start reading from
	PVOID pBuff; // return value
	ULONG Size; // size of memory to read
} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG ProcessId; //target process id
	ULONG Address; // address of memory to start reading from
	PVOID pBuff; // return value
	ULONG Size; // size of memory to read
} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

class KInterface
{
private:
	HANDLE hDriver; // Handle to driver
	DWORD ProcessID = 0;

	std::string StrRead(ULONG ReadAddress)
	{
		/*24 + null terminator
		only 24 to save memory and most of the strings won't go above it*/
		char buff[25];
		ZeroMemory(buff, 25);
		buff[24] = '\0';
		for (int i = 0; i < 24; i += 4)
		{
			DWORD addy = Read<DWORD>(ReadAddress + i);
			memcpy(&buff[i], &addy, 4);
			//if last read char is null terminator or wrong char
			//wont always work with special chars in player names
			if (buff[i + 3] <= 0 || buff[i + 3] > 127)
			{
				buff[i + 3] = '\0';
				break;
			}
		}
		return std::string(buff);

		////should be faster since its 1 read per 16 bytes and DWORD is 4 reads for 16
		//char buff[24];
		//Read(ReadAddress, buff, 24);
		//int k = 0;
		//for (int i = 8; i < 24; i++)
		//{
		//	if (buff[i] <= 0 || buff[i] > 127)
		//	{
		//		buff[i] = '\0';
		//		k = i;
		//		break;
		//	}
		//}
		//char str[16];
		//memcpy(&str[0], &buff[8], k);
		//return std::string(str);
	}

	std::string WStrRead(DWORD ReadAddress)
	{
		char buff[24];
		Read(ReadAddress, buff, 24);
		int k = 0;
		for (int i = 8; i < 24; i++)
		{
			if (buff[i] == '\0')
			{
				k = i;
				break;
			}
		}
		char str[16];
		memcpy(&str[0], &buff[8], k);
		return std::string(str);
	}

public:

	// Initializer
	KInterface(LPCSTR RegistryPath)
	{
		hDriver = CreateFileA(RegistryPath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			0,
			0);

		ProcessID = Process();
	}

	DWORD Process()
	{
		if (ProcessID)
			return ProcessID;

		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Id = 0;
		DWORD Bytes = 0;

		if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id), &Id, sizeof(Id), &Bytes, NULL))
			return Id;
		else
			return false;
	}

	DWORD GetClientModule()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address = 0;
		DWORD Bytes = 0;

		if (DeviceIoControl(hDriver, IO_GET_MODULE_REQUEST, &Address, sizeof(Address),
			&Address, sizeof(Address), &Bytes, NULL))
			return Address;
		else
			return false;
	}

	template <typename type>
	type Read(ULONG ReadAddress, SIZE_T Size = sizeof(type))
	{
		//type cData;
		if (hDriver == INVALID_HANDLE_VALUE)
			return (type)false;

		//DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessID;
		ReadRequest.Address = ReadAddress;
		//ReadRequest.pBuff = &cData;
		ReadRequest.Size = Size;

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
			return  *(type*)&ReadRequest.pBuff;

		return (type)false;
	}

	//for some reason it can only read up to 16 bytes starting from 8th
	void Read(ULONG ReadAddress, void* structure, SIZE_T Size)
	{
		//type cData;
		if (hDriver == INVALID_HANDLE_VALUE)
			return;
		//DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessID;
		ReadRequest.Address = ReadAddress;
		//ReadRequest.pBuff = structure;
		ReadRequest.Size = Size;

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), structure, Size, 0, 0))
		{
			return;
		}

		return;
	}

	// 0 - normal, 1 - pointer
	std::string ReadString(DWORD address, bool checkASCII = false, int noPtr = -1)
	{
		if (noPtr == 0)
			return StrRead(address);
		else if (noPtr == 1)
			return StrRead(Read<DWORD>(address, sizeof(DWORD)));

		std::string strReturn;

		int max = Read<int>(address + 0x14); // read string length
		int max2 = Read<int>(address + 0x08);
		//clog.AddLog("%x : %i", address, max);

		unsigned char buf = Read<unsigned char>(address, 1);

		bool temp = 0;
		if (max < 16 && max > 0 && (buf > 0 && buf <= 128) && !(max2 > 0 && max2 < 256))
		{
			strReturn = StrRead(address);
			temp = 0;
		}
		else
		{
			strReturn = StrRead(Read<DWORD>(address, sizeof(DWORD)));
			temp = 1;
		}

		// be extra sure
		if (checkASCII)
		{
			if (!utils->ContainsOnlyASCII(strReturn))
			{
				if (temp == 0)
					return StrRead(Read<DWORD>(address, sizeof(DWORD)));
				else
					return StrRead(address);
			}
		}

		return strReturn;
	}

	std::wstring ReadWString(DWORD address)
	{
		int max = Read<int>(address + 0x14); // read string length
		int max2 = Read<int>(address + 0x08);
		//clog.AddLog("%x : %i", address, max);

		bool temp = 0;
		if (max < 16 && max > 0 && !(max2 > 0 && max2 < 256))
		{
			return utils->StringToWstring(WStrRead(address));
		}
		else
		{
			return utils->StringToWstring(WStrRead(Read<DWORD>(address, sizeof(DWORD))));
		}
	}

	/*bool Write(ULONG WriteAddress, ULONG WriteValue, SIZE_T WriteSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
		DWORD Bytes;

		KERNEL_WRITE_REQUEST  WriteRequest;
		WriteRequest.ProcessId = ProcessID;
		WriteRequest.Address = WriteAddress;
		WriteRequest.pBuff = &WriteValue;
		WriteRequest.Size = WriteSize;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest),
			0, 0, &Bytes, NULL))
			return true;
		else
			return false;
	}*/
};

static KInterface Memory(R"(\\.\kbotl)");

static DWORD ClientAddress = Memory.GetClientModule();

#endif //!_KINTERFACE_H_