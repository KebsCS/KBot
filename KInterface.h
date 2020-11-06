#pragma once


#ifndef _KINTERFACE_H_
#define _KINTERFACE_H_

#pragma warning (disable: 4302 4311 4244 4700 )

#include <Windows.h>
#include <apiquery2.h>
#include <string>

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

	std::string StrRead(ULONG ReadAddress)
	{
		BYTE bytes[24];
		bool invalidChar = false;
		for (int i = 0; i < 24; i += 4)
		{
			//todo rewrite to only read addres once for all the bytes / read string instantly
			BYTE byte[4];
			float buf = Read<float>(ReadAddress + i, 4); //read bytes from address in memory
			memcpy(&byte, (unsigned char*)(&buf), 4); // transforms float bytes char bytes


			for (int j = 0; j < 4; j++)
			{

				bytes[i + j] = byte[j]; // adds chars to bytes array making a string 
				if ((char)byte[j] < 0 || (char)byte[j] >= 128)
				{
					memcpy(&byte[j], "\0", 1); 
					invalidChar = true;
					break;
				}
			}
			if(invalidChar)
				break;
		}
		char str[sizeof(bytes) + 1];
		memcpy(str, bytes, sizeof(bytes)); // transforms bytes char array to actual char string
		if (str == NULL)
			return "Unknown";
		str[sizeof(bytes)] = '\0';
		//std::string sString = str;
		return str;
	}
public:
	HANDLE hDriver; // Handle to driver
	DWORD ProcessID;

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

		if (hDriver != INVALID_HANDLE_VALUE)
		{
			ULONG Id;
			DWORD Bytes;

			if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id), &Id, sizeof(Id), &Bytes, NULL))
			{
				ProcessID = Id;
			}
		}

	}

	DWORD Process()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Id;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id), &Id, sizeof(Id), &Bytes, NULL))
		{
			//ProcessID = Id;
			return Id;
		}
		else
			return false;
	}

	DWORD GetClientModule()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address;
		DWORD Bytes;

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


	std::string ReadString(DWORD address)
	{
		std::string strReturn;

		//todo string offsets from string class instead of memory read
		int max = Read<int>(address + 0x14); // read string length
		int max2 = Read<int>(address + 0x08);
		//clog.AddLog("%x : %i", address, max);

		unsigned char buf = Read<unsigned char>(address , 1); 


		if (max < 16 && max > 0 && (buf > 0 && buf <= 128) && !(max2 > 0 && max2 < 256))
		{
			strReturn = StrRead(address);

		}
		else
		{
			strReturn = StrRead(Read<int>(address, sizeof(int)));
		}
		//if ((max >= 16 && max > 0) || (max2 > 0 && max2 < 256)) //if invalid 
		//{
		//	strReturn = StrRead(Read<int>(address, sizeof(int))); // read pointer to the string

		//}
		//else
		//{
		//	strReturn = StrRead(address);
		//}
		return strReturn;

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



#endif //!_KINTERFACE_H_