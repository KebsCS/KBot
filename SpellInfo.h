#pragma once

#include "KInterface.h"






class SpellData
{
private:
	DWORD base;
	DWORD ProcessId = Driver.GetProcessId();
public:
	//char* GetMissileName() {
	//	return GetStr((DWORD)this + 0x0058);
	//}
	//char* GetSpellName() {
	//	return GetStr((DWORD)this + 0x007C);
	//}
	//char* GetDescription() {
	//	return GetStr((DWORD)this + 0x0088);
	//}
	//float GetEffectAmount() {
	//	return *(float*)((DWORD)this + 0xD0);
	//}

	//float GetIncreaseDamage() {
	//	return *(float*)((DWORD)this + 0xEC);
	//}

	//float GetSpellDuration() {
	//	return *(float*)((DWORD)this + 0x108);
	//}

	//float GetRootDuration() {
	//	return *(float*)((DWORD)this + 0x15C);
	//}
	//float GetIncreaseDamageBonus() {
	//	return *(float*)((DWORD)this + 0x178);
	//}
	//float GetCoefficient() {
	//	return *(float*)((DWORD)this + 0x200);
	//}
	//float GetCoefficient2() {
	//	return *(float*)((DWORD)this + 0x204);
	//}
	//int GetMaxHighlightTargets() {
	//	return *(int*)((DWORD)this + 0x208);
	//}



	float GetSpellInfo()
	{
		return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x280, sizeof(float));
	}

	//float GetDelayCastOffsetPercent() {
	//	return *(float*)((DWORD)this + 0x29C);
	//}

	//float GetDelayTotalTimePercent() {
	//	return *(float*)((DWORD)this + 0x2A0);
	//}

	//int GetMaxAmmo() {
	//	return *(int*)((DWORD)this + 0x31C);
	//}
	//int GetAmmoUsed() {
	//	return *(int*)((DWORD)this + 0x338);
	//}

	//float GetAmmoRechargeTime() {
	//	return *(float*)((DWORD)this + 0x354);
	//}

	//float GetMissileSpeed() {
	//	return *(float*)((DWORD)this + 0x450);
	//}
};


class SpellInfo
{
private:
	DWORD base;
	DWORD ProcessId = Driver.GetProcessId();
public:
	SpellData* GetSpellData()
	{
		return Driver.ReadVirtualMemory<SpellData*>(ProcessId, base + 0x44, sizeof(SpellData*));
	}

	SpellData* GetSpellClassByID(int ID)
	{
		return Driver.ReadVirtualMemory<SpellData*>(ProcessId, base + 0x478 + (0x4*ID), sizeof(SpellData*));
	}


};


class SpellSlot
{
private:
	DWORD base;
	DWORD ProcessId = Driver.GetProcessId();
public:
	//int GetLevel() {
	//	return *(int*)((DWORD)this + 0x20);
	//}

	//float GetTime() {
	//	return *(float*)((DWORD)this + 0x28);
	//}

	//float GetCD() {
	//	return *(float*)((DWORD)this + 0x18);
	//}

	SpellInfo* GetSpellInfo()
	{
		return Driver.ReadVirtualMemory<SpellInfo*>(ProcessId, base + 0x124, sizeof(SpellInfo*)); //oMissileSpellInfo
	}


};