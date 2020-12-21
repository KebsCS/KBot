#pragma once

#ifndef _OFFSETS_H_
#define _OFFSETS_H_


#define oObjAtkRange 0x1298 // 10.25 loveto5014
#define oUnderMouseObject 0x1BFD160 // 		8B 0D ? ? ? ? 89 0D // dword_[offset]
#define oLocalPlayer 0x34FA11C //			A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56 // dword_[offset]
#define oGameTime 0x34F22F0 // 				F3 0F 11 05 ? ? ? ? 8B 49 // dword_[offset]
#define oObjManager 0x1C5D610 //			8B 0D ? ? ? ? 89 74 24 14 // dword_[offset]



#define oViewMatrix 0x3520038 //B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC 
#define oHeroList 0x1C5B580 //				A1 ? ? ? ? 53 55 56 8B 68 04 8B 40 08 57 33 FF
#define oMinionList 0x28A9C24//				A1 ? ? ? ? 8B 50 04 8D 48 04 89 44 24 1C				//E8 ? ? ? ? EB 02 33 C0 6A 14 A3 ? ? ? ? E8 ? ? ? ? 83 C4 04 
#define oTurretList 0x34F12B8//				8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 13 8B 10
#define oInhibitorList 0x34FA89C//			8B 15 ? ? ? ? 8B 7A 04 8B 42 08 8D 0C 87 3B F9 73 31 66 90
#define oStructureList 0x1C5AF2C // FF 30 8B 0D ? ? ? ? E8 ? ? ? ? 
//#define oShopList 0x28BDEA0


#define UnitComponentInfo 0x2F1C
#define UCIPropertiesInstance 0x1C


//28C9344

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
#define oObjNetworkID 0xCC //
#define oObjPos 0x1D8 // 10.25 loveto5014
#define oObjVisibility 0x270
#define oObjHealth 0xD98 // 10.25 loveto5014
#define oObjMaxHealth 0xDA8
#define oObjMana 0x298
#define oObjMaxMana 0x2A8
#define oObjArmor 0x1278 // 10.25 loveto5014
#define oObjMagicRes 0x1280 // 10.25 loveto5014
#define oObjBonusMagicRes 0x1284 // 10.25 loveto5014
#define oObjBaseAtk 0x1250 //10.25 loveto5014
#define oObjBonusAtk 0x11D0 // 10.25 loveto5014
#define oObjMoveSpeed 0x1290 // 10.25 loveto5014
#define oObjAtkRange 0x1298 // 10.25 loveto5014
#define oObjBuffMgr 0x213C // 10.25 serge
#define oObjSpellBook 0x26E8 // 10.25 serge (0x2B60 - 0x478)
#define oObjChampionName 0x310C // 10.25 me
#define oObjLevel 0x3694 // 10.25 me
#define oObjSourceIndex 0x290
//#define oObjPerk1 0x3880  // 8D 8E ? ? ? ? E8 ? ? ? ? 8B CE E8 ? ? ? ? A1 // esi+[offset]h // 81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 53 8B 9C 24 ? ? ? ? 56 8B F1 88 5C 24 0C // fn + 19F
//#define oObjPerk2 0x2C8 // B 51 53 8B D9 33 C9 + 1B1 // go to sub_XXXX then go to sub_XXXX + 1F
#define oInhiRemainingRespawnTime 0xFF4
//ObjectManager
#define objectArray 0x14

//MissileInfo
#define oMissileSpellInfo 0x230
#define oMissileTargetIndex 0x2E8
#define oMissileStartPos 0x2A8
#define oMissileEndPos 0x2B4


#define mSummonerSpell1 0x3B58
#define mSummonerSpell2 0x3B64
#define mKeystone1 0x3B78 //perk_displayname
#define mKeystone2 0x3B84 //perk_tooltip

#define oObjFloatLethality 0x1194
#define oObjAbilityPower  0x11E0
#define oObjPercentArmorPen 0x117C
#define oObjCritChance 0x1274
#define oRecallState 0xD8C
#define oObjEXP 0x3684

//oSpellSlot
#define oSpellSlotRemainingCharge 0x58
#define oSpellSlotLevel 0x20
#define oSpellSlotRemainingRechargeCD 0x64
#define oSpellSlotTime 0x28
#define oSpellSlotCD 0x18
#define oSpellSlotDamage 0x94
#define oSpellSlotSpellInfo 0x134
#define oSpellSlotRemainingCD 0x28

#endif //!_OFFSETS_H_