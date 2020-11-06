#pragma once

#ifndef _OFFSETS_H_
#define _OFFSETS_H_

/*
Offset dumper by @Ph4nton
Wed Oct 28 20:45:23 2020
*/

#define BASEADDRESS GetModuleHandle(NULL)
#define oGetAttackDelay 0x2A5120	//8B 44 24 04 51 F3
#define oGetAttackCastDelay 0x2A5020	//83 EC 0C 53 8B 5C 24 14 8B CB 56 57 8B 03 FF 90
#define oDrawCircle 0x503470	//E8 ? ? ? ? 83 C4 1C 8B 7C 24 28
#define oIsAlive 0x19F2A0	//56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19
#define oIsInhib 0x1AD930	//E8 ? ? ? ? 83 C4 04 84 C0 74 38
#define oIsNexus 0x0	//E8 ? ? ? ? 57 88 44 24 20
#define oIsTurret 0x1ADD20	//E8 ? ? ? ? 83 C4 04 84 C0 74 09 5F
#define oIsMinion 0x1ADAF0	//E8 ? ? ? ? 83 C4 04 80 7F 26 06
#define oIsDragon 0x171C70	//83 EC 10 A1 ? ? ? ? 33 C4 89 44 24 0C 56 81 C1 ? ? ? ?
#define oIsBaron 0x173B20	//E8 ? ? ? ? 84 C0 74 0A BB ? ? ? ?
#define oIsHero 0x1ADAB0	//E8 ? ? ? ? 83 C4 04 84 C0 74 2B
#define oIsMissile 0x0	//E8 ? ? ? ? 83 C4 04 84 C0 74 1C FF
#define oIssueOrder 0x173BD0	//81 EC ? ? ? ? 56 57 8B F9 C7
#define oGetSpellState 0x5004F0	//E8 ? ? ? ? 8B F8 8B CB 89
#define oGetBasicAttack 0x166970	//53 8B D9 B8 ? ? ? ? 8B 93
#define oIsNotWall 0x9036A0	//E8 ? ? ? ? 33 C9 83 C4 0C 84
#define oGameVersion 0x57C060	//8B 44 24 04 BA ? ? ? ? 2B D0
#define oWorldToScreen 0x986720	//83 EC 10 56 E8 ? ? ? ? 8B 08
#define oGetFirstObject 0x288160	//8B 41 14 8B 51 18
#define oGetNextObject 0x289140	//E8 ? ? ? ? 8B F0 85 F6 75 E4
#define oLocalPlayer 0x3518034	//A1 ? ? ? ? 85 C0 74 07 05 ? ? ? ? EB 02 33 C0 56
#define oObjManager 0x1C7BB18	//8B 0D ? ? ? ? E8 ? ? ? ? FF 77
#define oGameTime 0x3510778	//F3 0F 11 05 ? ? ? ? 8B 49
#define oHudInstance 0x1C7BB54	//8B 0D ? ? ? ? 6A 00 8B 49 34 E8 ? ? ? ? B0
#define oRenderer 0x3540C74	//8B 15 ? ? ? ? 83 EC 08 F3
#define oD3DRenderer 0x35439B8	//A1 ? ? ? ? 89 54 24 18
#define oZoomClass 0x350FF54	//A3 ? ? ? ? 83 FA 10 72 32
#define oNetClient 0x351F884	//8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B
#define oUnderMouseObject 0x1C1B0F0 	//8B 0D ? ? ? ? 89 0D ? ? ? ? 3B 44 24 30
#define oGameInfo 0x35118B0	//A1 ? ? ? ? 83 78 08 02 0F 94 C0
#define oViewMatrix 0x68	//8D 4A ? 51 81 C2 ? ? ? ? 52
#define oProjection 0xA8	//8D 4A ? 51 81 C2 ? ? ? ? 52
#define oObjAttackRange 0x12D0	//D8 81 ? ? ? ? 8B 81 ? ? ? ?




#define oHeroList 0x1C79A48 //				A1 ? ? ? ? 53 55 56 8B 68 04 8B 40 08 57 33 FF
#define oMinionList 0x28C8100//				A1 ? ? ? ? 8B 50 04 8D 48 04 89 44 24 1C				
#define oTurretList 0x350F720//				8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 13 8B 10
#define oInhibitorList 0x35191C0//			8B 15 ? ? ? ? 8B 7A 04 8B 42 08 8D 0C 87 3B F9 73 31 66 90
#define oStructureList 0x1C794DC // 
#define oShopList 0x28C92E8
#define oTurretList2 0x350F71C
#define oObjectList 0x28C80E0
#define oObjectList2 0x28C8108

//28C9344


#define oIsTargetable 0x1D4070 


#define mHP 0xDC4
#define mMaxHP 0xDD4
#define mHPMaxPenalty 0xDE4
#define mAllShield 0xE04
#define mPhysicalShield 0xE14
#define mMagicalShield 0xE24
#define mChampSpecificHealth 0xE34
#define mStopShieldFade 0xE54
#define mGoldTotal 0x1B38


#define oObjBuffMgr 0x2174
#define oObjSpellBook 0x2720
#define oObjChampionName 0x314C
#define oObjLevel 0x36BC
#define oObjSourceIndex 0x290

#define oMissileSpellInfo 0x230
#define oMissileTargetIndex 0x2E8
#define oMissileStartPos 0x2A8
#define oMissileEndPos 0x2B4




#endif //!_OFFSETS_H_