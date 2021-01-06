#pragma once

#ifndef _OFFSETS_H_
#define _OFFSETS_H_

//#define oUnderMouseObject 0x1BFD160 // 		8B 0D ? ? ? ? 89 0D // dword_[offset]
#define oLocalPlayer 0x34EEDE4 //			A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56 // dword_[offset]
#define oGameTime 0x34E6FD4 // 				F3 0F 11 05 ? ? ? ? 8B 49 // dword_[offset]
#define oObjManager 0x1C52490 //			8B 0D ? ? ? ? 89 74 24 14 // dword_[offset]
#define oHudInstance 0 // TODO

#define oViewMatrix 0x3514BE8 //B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC
#define oHeroList 0x1C50400 //				A1 ? ? ? ? 53 55 56 8B 68 04 8B 40 08 57 33 FF
#define oMinionList 0x289E8EC//				A1 ? ? ? ? 8B 50 04 8D 48 04 89 44 24 1C				//E8 ? ? ? ? EB 02 33 C0 6A 14 A3 ? ? ? ? E8 ? ? ? ? 83 C4 04
#define oTurretList 0x34E5F88//				8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 13 8B 10
#define oInhibitorList 0x34EF49C//			8B 15 ? ? ? ? 8B 7A 04 8B 42 08 8D 0C 87 3B F9 73 31 66 90
#define oStructureList 0x1C4FDA8 // FF 30 8B 0D ? ? ? ? E8 ? ? ? ?
//#define oShopList 0x28BDEA0

#define oMissileMap 0x34ED13C // 8B 15 ? ? ? ? 85 D2 74 44  //mov edx, offset

//
//#define oIsTargetable 0x1281714
//
//#define oObjNetworkID 0xCC //
//#define mHP 0xD98
//#define mMaxHP 0xDA8
//#define mHPMaxPenalty 0xDE4
//#define mAllShield 0xE04
//#define mPhysicalShield 0xE14
//#define mMagicalShield 0xE24
//#define mChampSpecificHealth 0xE34
//#define mStopShieldFade 0xE54
//#define mGoldTotal 0x1AF0
//
//
//#define oObjBuffMgr 0x213C
//#define oObjSpellBook 0x26E8 //8B 44 24 08 81 C1 //ecx, 2720h
//#define oObjChampionName 0x3308
//#define oObjLevel 0x366C
//#define oObjSourceIndex 0x290
//
//#define oMissileSpellInfo 0x230
//#define oMissileTargetIndex 0x2E8
//#define oMissileStartPos 0x2A8
//#define oMissileEndPos 0x2B4

//CObject
#define oObjIndex 0x20
#define oObjTeam 0x4C
#define oObjName 0x6C
#define oObjNetworkID 0xCC
#define oObjPos 0x1D8
#define oObjVisibility 0x270
#define oObjMana 0x298
#define oObjMaxMana 0x2A8

#define oRecallState 0xD8C
#define oObjHealth 0xD98
#define oObjMaxHealth 0xDA8

#define oObjFlatMagicPen 0x1198
#define oObjPercentArmorPen 0x119C
#define oObjPercentMagicPen 0x11A0
#define oObjFloatLethality 0x11B4
#define oObjBonusAtk 0x11F0
#define oObjAbilityPower  0x1200
#define oObjLifeSteal 0x125C
#define oObjBaseAtk 0x1270
#define oObjCritChance 0x1294
#define oObjArmor 0x1298
#define oObjBonusArmor 0x129C
#define oObjMagicRes 0x12A0
#define oObjBonusMagicRes 0x12A4
#define oObjMoveSpeed 0x12B0
#define oObjAtkRange 0x12B8

//#define oObjBuffMgr 0x213C
#define oObjSpellBook 0x2708 // (0x2B60 - 0x478)

#define UnitComponentInfo 0x2F44
#define UCIPropertiesInstance 0x1C

#define oObjChampionName 0x3134

#define oObjEXP 0x36AC
#define oObjLevel 0x36BC
#define oObjAvaiableSpellPoints 0x36E4

#define oObjSummonerSpell1 0x3B80
#define oObjSummonerSpell2 0x3B8C
#define oObjKeystone1 0x3BA0 //perk_displayname
#define oObjKeystone2 0x3BAC //perk_tooltip

#define oInhiRemainingRespawnTime 0x1014

//UCIProperties
#define oSelectionRadius 0x044C
#define oPathingRadius 0x0450
#define oBoundingRadius 0x0454

//#define oObjPerk1 0x3880  // 8D 8E ? ? ? ? E8 ? ? ? ? 8B CE E8 ? ? ? ? A1 // esi+[offset]h // 81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 53 8B 9C 24 ? ? ? ? 56 8B F1 88 5C 24 0C // fn + 19F
//#define oObjPerk2 0x2C8 // B 51 53 8B D9 33 C9 + 1B1 // go to sub_XXXX then go to sub_XXXX + 1F

//MissileInfo
#define oMissileSpellInfo 0x230
#define oMissileSourceIndex 0x290
#define oMissileTargetIndex 0x2E8
#define oMissileStartPos 0x2A8
#define oMissileEndPos 0x2B4

#define oSpellInfoSpellData 0x44

//oSpellSlot

#define oSpellSlotLevel 0x20
#define oSpellSlotTime 0x28
#define oSpellSlotRemainingCharge 0x58
#define oSpellSlotDamage 0x94

//#define oSpellSlotCD 0x18

//#define oSpellSlotRemainingRechargeCD 0x64

//#define oSpellSlotSpellInfo 0x134

#endif //!_OFFSETS_H_