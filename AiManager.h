#pragma once

#ifndef _AIMANAGER_H_
#define _AIMANAGER_H_

#include "KInterface.h"
#include "offsets.h"

class AiManager
{
public:

	bool IsMoving()
	{
		return Memory.Read<bool>((DWORD)this + oAiManagerIsMoving, sizeof(bool));
	}

	Vector3 GetTargetPos()
	{
		return Vector3(Memory.Read<float>((DWORD)this + oAiManagerTargetPos, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerTargetPos + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerTargetPos + 0x8, sizeof(float)));
	}

	Vector3 GetServerPos()
	{
		return Vector3(Memory.Read<float>((DWORD)this + oAiManagerServerPos, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerServerPos + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerServerPos + 0x8, sizeof(float)));
	}

	//todo
	//Vector3 GetNavBegin()
	//{
	//	DWORD ptr = Memory.Read<DWORD>((DWORD)this + oAiManagerNavBegin);
	//	return Vector3(Memory.Read<float>(ptr, sizeof(float)),
	//		Memory.Read<float>(ptr + 0x4, sizeof(float)),
	//		Memory.Read<float>(ptr + 0x8, sizeof(float)));
	//}

	Vector3 GetNavEnd()
	{
		return Vector3(Memory.Read<float>((DWORD)this + oAiManagerNavEnd, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerNavEnd + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + oAiManagerNavEnd + 0x8, sizeof(float)));
	}

	int GetPassedWaypoints()
	{
		return Memory.Read<int>((DWORD)this + oAiManagerPassedWaypoints, sizeof(int));
	}

	bool IsDashing()
	{
		return Memory.Read<bool>((DWORD)this + oAiManagerIsDashing, sizeof(bool));
	}

	float GetDashSpeed()
	{
		return Memory.Read<float>((DWORD)this + oAiManagerDashSpeed, sizeof(float));
	}
};

#endif // !_AIMANAGER_H_
