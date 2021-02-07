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
		return Memory.Read<bool>((DWORD)this + Offsets::oAiManagerIsMoving, sizeof(bool));
	}

	Vector3 GetTargetPos()
	{
		return Vector3(Memory.Read<float>((DWORD)this + Offsets::oAiManagerTargetPos, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerTargetPos + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerTargetPos + 0x8, sizeof(float)));
	}

	Vector3 GetServerPos()
	{
		return Vector3(Memory.Read<float>((DWORD)this + Offsets::oAiManagerServerPos, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerServerPos + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerServerPos + 0x8, sizeof(float)));
	}

	Vector3 GetNavBegin()
	{
		return Vector3(Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavBegin, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavBegin + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavBegin + 0x8, sizeof(float)));
	}

	Vector3 GetNavEnd()
	{
		return Vector3(Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavEnd, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavEnd + 0x4, sizeof(float)),
			Memory.Read<float>((DWORD)this + Offsets::oAiManagerNavEnd + 0x8, sizeof(float)));
	}

	Vector3 GetVelocity()
	{
		return Memory.Read<Vector3>((DWORD)this + Offsets::oAiManagerVelocity);
	}

	int GetPassedWaypoints()
	{
		return Memory.Read<int>((DWORD)this + Offsets::oAiManagerPassedWaypoints, sizeof(int));
	}

	bool IsDashing()
	{
		return Memory.Read<bool>((DWORD)this + Offsets::oAiManagerIsDashing, sizeof(bool));
	}

	float GetDashSpeed()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oAiManagerDashSpeed, sizeof(float));
	}
};

#endif // !_AIMANAGER_H_
