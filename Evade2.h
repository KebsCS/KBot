#pragma once

#ifndef _EVADE2_H_
#define _EVADE2_H_

#include "Geometry.h"
#include "Menu.h"

#include "CObjectManager.h"
#include "CSpellEntry.h"
#include "CMissileClient.h"

class Evade2
{
private:

	//int IsNotWall(Vector3 a1)
	//{
	//	float v1; // xmm1_4
	//	int v2; // ecx
	//	int v3; // edx
	//	signed int v4; // edi
	//	float v5; // xmm0_4
	//	int v6; // eax
	//	DWORD v7; // eax
	//	v1 = Memory.Read<float>(ClientAddress + 0x350A0E0 + 1456);
	//	v2 = Memory.Read<DWORD>(ClientAddress + 0x350A0E0 + 1444);
	//	v3 = v2 - 1;
	//	v4 = (signed int)(float)((float)(a1.z - Memory.Read<float>(ClientAddress + 0x350A0E0 + 108)) * v1);
	//	v5 = (float)(a1.x - Memory.Read<float>(ClientAddress + 0x350A0E0 + 100)) * v1;
	//	if ((signed int)v5 <= v2 - 1)
	//	{
	//		v3 = (signed int)v5;
	//		if ((signed int)v5 < 0)
	//			v3 = 0;
	//	}
	//	v6 = Memory.Read<DWORD>(ClientAddress + 0x350A0E0 + 1448) - 1;
	//	if (v4 <= v6)
	//	{
	//		v6 = (signed int)(float)((float)(a1.z - Memory.Read<float>(ClientAddress + 0x350A0E0 + 108)) * v1);
	//		if (v4 < 0)
	//			v6 = 0;
	//	}
	//	v7 = (Memory.Read<DWORD>(ClientAddress + 0x350A0E0 + 128) + 8 * (v3 + v6 * v2));
	//	if (!v7)
	//		return 0;
	//	if (Memory.Read < DWORD>(v7))
	//		return Memory.Read<DWORD>(Memory.Read<DWORD>(v7) + 6);
	//	return Memory.Read<DWORD>(v7 + 2);
	//}

	enum SlotID :int
	{
		_Q = 0,
		_W = 1,
		_E = 2,
		_R = 3,
		_D = 4,
		_F = 5
	};

	struct Spell
	{
		std::string name;
		std::string icon;
		std::string displayName;
		std::string missileName;
		int slot;
		int type;
		float speed;
		float range;
		float delay;
		float radius;
		float angle;
		int danger;
		bool cc;
		bool collision;
		bool windwall;
		bool hitbox;
		bool fow;
		bool exception;
		bool extend;
		bool enabled = true;
		bool useMissile = false;

		// After detection
		float startTime;
		Vector3 startPos;
		Vector3 endPos;
		Vector3 currPos;
		Geometry::Polygon path;
		CSpellEntry spell;
		CMissileClient missile;
		CObject obj;
	};

	struct Champ
	{
		// Champ name
		std::string hero;
		// Object
		CObject obj;
		// Spells from DB
		std::vector<Spell>spells;
	};
	std::vector<Champ>SpellDB;
	std::vector<Champ>ChampsInGame;

	struct EvadeSpell
	{
		std::string name;
		std::string icon;
		std::string displayName;
		int type;
		int danger;
		float range;
		int slot;
		bool use = true;
	};

	struct EvadeChamp
	{
		std::string hero;
		std::vector<EvadeSpell>spells;
	};
	std::vector< EvadeChamp>EvadeSpellDB;
	std::vector<EvadeSpell>MyEvadeSpells;

	std::vector<Spell>DetectedSkillshots;
	std::vector<Spell>EnabledSkillshots;
	std::vector<Spell>DangerSkillshots;

private:
	float BoundingRadius = 65;
	float GameTimer = 0;
	//float MathHuge = 999999.f; //temporary

private:
	Vector3 MyHeroPos;
	Vector3 PreviousPos;
	Vector3 EvadePoint;
	Vector3 EvadeToPoint;

	bool IsEvading = false;
	bool Force = false;
	bool NoSolutionFound = false;
	float LastClick;

private:
	void Init();
	void InitSpells();
	void InitWorldMap();

	std::vector<Geometry::Polygon> WorldMap;
	bool InWall(Vector3 pos);

public:

	Evade2()
	{
		InitSpells();
		Init();
		//InitWorldMap();
	}

	void Tick();
	void StartEvading();
	void Draw();

	void MoveToPos(Vector3 pos, bool stop = false);
	void SetEvading(bool b);
	bool IsPosSafe(Vector3 pos);

	void OnCreateMissile(Spell& spell);
	void OnSpellCast(Spell& spell);
	Vector3 CalculateEndPos(Spell& spell);
	Geometry::Polygon GetPath(Spell& spell);

	std::vector<Vector3> GetPossibleEvadePoints();
	Vector3 GetEvadingPosition();

	Vector3 GetMovePath();
	int CoreManager(Spell s);
	Vector3 GetBestEvadePos(std::vector<Spell> spells, float radius, int mode, int extra, bool force);

	bool IsValid(Spell& spell);

	Geometry::Polygon testPoly;
	void TestPoints();

	//Vector3 ExtendedPos = 0;
	//Vector3 SafePos = 0;
	//bool Evading = 0;
};

#endif