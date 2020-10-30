#pragma once
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


class String
{
	char        _content[0x10];
	int         _contentLength = 0;
	int         _maxContentLength = 0;

public:
	operator std::string(void)
	{
		return std::string(Get());
	}

	char* Get(void)
	{
		if (DWORD(this) <= 0x1000)
			return (char*)"";

		return _maxContentLength >= 0x10
			? *reinterpret_cast<char**>(_content)
			: _content;
	}
private:
	operator const char* (void)
	{
		return Get();
	}
};

class KInterface
{
public:
	HANDLE hDriver; // Handle to driver

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
	}

	DWORD GetProcessId()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Id;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id), &Id, sizeof(Id), &Bytes, NULL))
			return Id;
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
	type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress,
		SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return (type)false;

		//DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = Size;

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
			return *(type*)&ReadRequest.pBuff;
		else
			return (type)false;
	}


	std::string ReadStringMemory(ULONG ProcessId, ULONG ReadAddress,
		SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return (std::string)nullptr;

		//DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = Size;

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0)) 
			return *(String*)&ReadRequest.pBuff;
		else
			return (std::string)nullptr;
	}

	/*char* ReadStringMemory(ULONG ProcessId, ULONG ReadAddress,
		SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return (char*)nullptr;

		DWORD Return, Bytes;
		char* name[100];
		KERNEL_READ_REQUEST ReadRequest;
		ReadRequest.pBuff = name;
		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = Size;

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
			return *(char**)&ReadRequest.pBuff;
		else
			return (char*)nullptr;
	}*/

	bool WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress,
		ULONG WriteValue, SIZE_T WriteSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
		DWORD Bytes;

		KERNEL_WRITE_REQUEST  WriteRequest;
		WriteRequest.ProcessId = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.pBuff = &WriteValue;
		WriteRequest.Size = WriteSize;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest),
			0, 0, &Bytes, NULL))
			return true;
		else
			return false;
	}

};


