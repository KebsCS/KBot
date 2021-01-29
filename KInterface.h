#pragma once

#ifndef _KINTERFACE_H_
#define _KINTERFACE_H_

#pragma warning (disable: 4302 4311 4244 4700 )

#include <Windows.h>
#include <apiquery2.h>
#include <string>

#pragma pack( push, 4 )
template<class Type = DWORD>
class CObfuscation
{
public:
	unsigned char	m_bIsFilled;
	unsigned char	m_bLengthXor32;
	unsigned char	m_bLengthXor8;
	Type			m_tKey;
	unsigned char	m_bIndex;
	Type			m_atValues[sizeof(Type) == 1 ? 3 : 4];
public:
	Type get()
	{
		__try
		{
			if (m_bIsFilled != 1)
				return Type(0);

			Type tResult = sizeof(Type) == 1 ? this->m_atValues[(this->m_bIndex + 1) & 3] : this->m_atValues[this->m_bIndex];
			if (this->m_bLengthXor32)
			{
				for (unsigned char i = 0; i < this->m_bLengthXor32; i++)
				{
					reinterpret_cast<PDWORD>(&tResult)[i] ^= ~(reinterpret_cast<PDWORD>(&this->m_tKey)[i]);
				}
			}
			if (this->m_bLengthXor8 && sizeof(Type) == 1)
			{
				for (unsigned char i = sizeof(Type) - this->m_bLengthXor8; i < sizeof(Type); i++)
				{
					reinterpret_cast<PBYTE>(&tResult)[i] ^= ~(reinterpret_cast<PBYTE>(&this->m_tKey)[i]);
				}
			}
			return tResult;
		}
		__except (true)
		{
			return Type(0);
		}
	}
};
#pragma pack( pop )

#pragma once
#include <inttypes.h>
#include <intrin.h>

#pragma pack( push, 4 )
template <typename t>
class xor_value {
	bool xor_key_was_init = 0;
	unsigned char bytes_xor_count;
	unsigned char bytes_xor_count_8;
	t xor_key;
	unsigned char value_index = 0;
	t values_table[4];
public:
	t decrypt() {
		if (xor_key_was_init != 1)
			return 0;

		auto xored_value = values_table[value_index];
		auto xor_key_value = xor_key;

		{
			auto xor_value_ptr = reinterpret_cast<uint32_t*>(&xor_key_value);
			for (auto i = 0; i < bytes_xor_count; i++)
				*(reinterpret_cast<uint32_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		{
			auto xor_value_ptr = reinterpret_cast<unsigned char*>(&xor_key_value);

			for (auto i = sizeof(t) - bytes_xor_count_8; i < sizeof(t); ++i)
				*(reinterpret_cast<unsigned char*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		return xored_value;
	}

	void encrypt(t value) {
		if (!xor_key_was_init) {
			if (sizeof(t) <= 2) {
				bytes_xor_count_8 = sizeof(t);
				bytes_xor_count = 0;
			}
			else {
				bytes_xor_count_8 = sizeof(t) % 4;
				bytes_xor_count = (sizeof(t) - bytes_xor_count_8) / 4;
			}

			auto key = __rdtsc();
			auto key_index = 0;

			for (auto i = 0; i < sizeof(t); i++) {
				*(reinterpret_cast<unsigned char*>(&xor_key) + i) = *(reinterpret_cast<unsigned char*>(&key) + key_index++);

				if (key_index == 8) {
					key = __rdtsc();
					key_index = 0;
				}
			}

			value_index = 0;
			xor_key_was_init = 1;
		}

		auto xored_value = value;
		auto xor_key_value = xor_key;

		{
			auto xor_value_ptr = reinterpret_cast<uint32_t*>(&xor_key_value);
			for (auto i = 0; i < bytes_xor_count; i++)
				*(reinterpret_cast<uint32_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		{
			auto xor_value_ptr = reinterpret_cast<unsigned char*>(&xor_key_value);

			for (auto i = sizeof(t) - bytes_xor_count_8; i < sizeof(t); ++i)
				*(reinterpret_cast<unsigned char*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		auto new_value_index = uint8_t(value_index + 1) & 3;

		values_table[new_value_index] = xored_value;

		value_index = new_value_index;
	}
};
#pragma pack( pop )

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

		//todo doesnt work when string is in pointer
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

	DWORD DeobfuscateMember(DWORD ReadAddress)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return 0;
		KERNEL_READ_REQUEST ReadRequest;
		ReadRequest.ProcessId = ProcessID;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = sizeof(CObfuscation<DWORD>);

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		{
			CObfuscation<DWORD> oObfuscation = *(CObfuscation<DWORD>*) & ReadRequest.pBuff;
			DWORD temp = oObfuscation.get();
			return Read<DWORD>(temp + 0x08);
		}
		return 0;
	}

	DWORD DeobfuscateMember2(DWORD ReadAddress)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return 0;
		KERNEL_READ_REQUEST ReadRequest;
		ReadRequest.ProcessId = ProcessID;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = sizeof(xor_value<DWORD>);

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		{
			xor_value<DWORD> oObfuscation = *(xor_value<DWORD>*) & ReadRequest.pBuff;
			DWORD temp = oObfuscation.decrypt();
			return Read<DWORD>(temp + 0x08);
		}
		return 0;
	}

	std::string ReadString(DWORD address, int noPtr = -1)
	{
		if (noPtr==0)
			return StrRead(address);
		else if(noPtr==1)
			return StrRead(Read<DWORD>(address, sizeof(DWORD)));

		std::string strReturn;

		//todo string offsets from string class instead of memory read
		int max = Read<int>(address + 0x14); // read string length
		int max2 = Read<int>(address + 0x08);
		//clog.AddLog("%x : %i", address, max);

		unsigned char buf = Read<unsigned char>(address, 1);

		if (max < 16 && max > 0 && (buf > 0 && buf <= 128) && !(max2 > 0 && max2 < 256))
		{
			strReturn = StrRead(address);
		}
		else
		{
			strReturn = StrRead(Read<DWORD>(address, sizeof(DWORD)));
		}

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

static KInterface Memory(R"(\\.\kbotl)");

static DWORD ClientAddress = Memory.GetClientModule();

#endif //!_KINTERFACE_H_