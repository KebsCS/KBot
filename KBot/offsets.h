#pragma once

#ifndef _OFFSETS_H_
#define _OFFSETS_H_

namespace Offsets
{
	//Main
	constexpr auto oUnderMouseObject = 0x2fb6218;
	constexpr auto oLocalPlayer = 0x2FB5814; //			A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56 // dword_[offset]
	constexpr auto oGameTime = 0x2fadb80; // 				F3 0F 11 05 ? ? ? ? 8B 49 // dword_[offset]
	constexpr auto oObjManager = 0x1719438; //			8B 0D ? ? ? ? 89 74 24 14 // dword_[offset]
	constexpr auto oHudInstance = 0x1719470; //			8B 0D ? ? ? ? FF 77 20 8B 49 14 // dword_[offset]
	constexpr auto oViewMatrix = 0x2fd92a8; //B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC

	//Object lists
	constexpr auto oHeroList = 0x17173d8; //				A1 ? ? ? ? 53 55 56 8B 68 04 8B 40 08 57 33 FF
	constexpr auto oMinionList = 0x2fa884c; //A1 ? ? ? ? 53 55 8B 6C 24 1C				A1 ? ? ? ? 8B 50 04 8D 48 04 89 44 24 1C				//E8 ? ? ? ? EB 02 33 C0 6A 14 A3 ? ? ? ? E8 ? ? ? ? 83 C4 04
	constexpr auto oTurretList = 0x2FACAF8;//				8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 13 8B 10
	constexpr auto oInhibitorList = 0x2FB6B8C; //			8B 15 ? ? ? ? 8B 7A 04 8B 42 08 8D 0C 87 3B F9 73 31 66 90
	//constexpr auto oStructureList = ;  // FF 30 8B 0D ? ? ? ? E8 ? ? ? ?
	constexpr auto oMissileMap = 0x2FB4BA8; // 8B 15 ? ? ? ? 85 D2 74 44  //mov edx, offset

	//Renderer
	constexpr auto oRenderer = 0x2fdc11c;	//8B 15 ? ? ? ? 83 EC 08 F3
	constexpr auto oRendererWidth = 0x10;
	constexpr auto oRendererHeight = 0x14;

	//Minimap
	constexpr auto oMinimapObject = 0x2fb5810; //8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? C7
	constexpr auto oMinimapObjectHud = 0x88;
	constexpr auto oMinimapHudPos = 0x60;
	constexpr auto oMinimapHudSize = 0x68;

	//E8 ?? ?? ?? ?? 50 8B CE E8 ?? ?? ?? ?? 80 BB ?? ?? ?? ?? ??
	//go into call function
	//this[offset]
	//offset-4
	constexpr auto oAiManager = 0x2D1C;

	//constexpr auto mHPMaxPenalty = 0xDE4;
	//constexpr auto mAllShield = 0xE04;
	//constexpr auto mPhysicalShield = 0xE14;
	//constexpr auto mMagicalShield = 0xE24;
	//constexpr auto mChampSpecificHealth = 0xE34;
	//constexpr auto mStopShieldFade = 0xE54;
	//constexpr auto mGoldTotal = 0x1AF0;

	//CObject
	constexpr auto oObjIndex = 0x20;
	constexpr auto oObjTeam = 0x4C;
	constexpr auto oObjName = 0x6C;
	constexpr auto oObjNetworkID = 0xCC;
	constexpr auto oObjPos = 0x1D8;
	constexpr auto oObjVisibility = 0x270;
	constexpr auto oObjMana = 0x298;
	constexpr auto oObjMaxMana = 0x2A8;

	constexpr auto oRecallState = 0xD8C;
	constexpr auto oObjHealth = 0xD98;
	constexpr auto oObjMaxHealth = 0xDA8;

	constexpr auto oObjActionState = 0x106C;

	constexpr auto oObjFlatMagicPen = 0x11C0;
	constexpr auto oObjPercentArmorPen = 0x11C4;
	constexpr auto oObjPercentMagicPen = 0x11C8;
	constexpr auto oObjFloatLethality = 0x11DC;
	constexpr auto oObjBonusAtk = 0x1218;
	constexpr auto oObjAbilityPower = 0x1228;
	constexpr auto oObjLifeSteal = 0x1284;
	constexpr auto oObjAttackSpeed = 0x129C;
	constexpr auto oObjBaseAtk = 0x12A0;
	constexpr auto oObjCritChance = 0x12C4;
	constexpr auto oObjArmor = 0x12C8;
	constexpr auto oObjBonusArmor = 0x12CC;
	constexpr auto oObjMagicRes = 0x12D0;
	constexpr auto oObjBonusMagicRes = 0x12D4;
	constexpr auto oObjHealthRegen = 0x12DC;
	constexpr auto oObjMoveSpeed = 0x12E0;
	constexpr auto oObjAtkRange = 0x12E8;

	constexpr auto oObjBuffMgr = 0x2254;
	constexpr auto oObjSpellBook = 0x2408; // (-0x478)

	constexpr auto UnitComponentInfo = 0x2C44;
	constexpr auto UCIPropertiesInstance = 0x1C;

	constexpr auto oObjChampionName = 0x2E34;

	constexpr auto oObjEXP = 0x33CC;
	constexpr auto oObjLevel = 0x33DC;
	constexpr auto oObjAvaiableSpellPoints = 0x3404;

	constexpr auto oObjSummonerSpell1 = 0x38A0;
	constexpr auto oObjSummonerSpell2 = 0x38AC;
	constexpr auto oObjKeystone1 = 0x38C0; //perk_displayname
	constexpr auto oObjKeystone2 = 0x38CC;//perk_tooltip

	constexpr auto oInhiRemainingRespawnTime = 0xFA4;
	constexpr auto oTurretName = 0xFA4;

	//UCIProperties
	constexpr auto oSelectionRadius = 0x044C;
	constexpr auto oPathingRadius = 0x0450;
	constexpr auto oBoundingRadius = 0x0454;
	constexpr auto oBaseAttackSpeed = 0x1D4;

	//MissileInfo not tested
	constexpr auto oMissileSpellInfo = 0x230;
	constexpr auto oMissileSlot = 0x234;
	constexpr auto oMissileLevel = 0x288;
	constexpr auto oMissileSourceIndex = 0x290;
	constexpr auto oMissileTargetIndex = 0x2E8;
	constexpr auto oMissileStartPos = 0x2A8;
	constexpr auto oMissileEndPos = 0x2B4;

	//SpellEntry
	constexpr auto oSpellEntrySpellInfo = 0x8;
	constexpr auto oSpellEntryIndex = 0x14;
	constexpr auto oSpellEntrySlot = 0xC;
	constexpr auto oSpellEntryStartPos = 0x80;
	constexpr auto oSpellEntryEndPos = 0x8C;
	constexpr auto oSpellEntryIsBasicAttack = 0xBC;
	constexpr auto oSpellEntryWindupTime = 0x4C0;
	constexpr auto oSpellEntryCastStartTime = 0x544;

	//SpellInfo
	constexpr auto oSpellInfoSpellName = 0x18;
	constexpr auto oSpellInfoSpellData = 0x44;

	//SpellData
	constexpr auto oSpellDataRange = 0x3C0;
	constexpr auto oSpellDataCastRadius = 0x3F8;
	constexpr auto oSpellDataSpeed = 0x44C;
	constexpr auto oSpellDataWidth = 0x474;
	constexpr auto oSpellDataMana = 0x524;
	//0x64 or 0x88 name?
	//

	//SpellSlot
	constexpr auto oSpellSlotLevel = 0x20;
	constexpr auto oSpellSlotCooldownExpire = 0x28;
	constexpr auto oSpellSlotRemainingCharge = 0x58;
	constexpr auto oSpellSlotIsActive = 0x74; // toggle (Jinx Q)
	constexpr auto oSpellSlotDamage = 0x94;
	constexpr auto oSpellSlotCooldown = 0xA4;
	constexpr auto oSpellSlotSpellInfo = 0x13C;
	//constexpr auto oSpellSlotRemainingRechargeCD = 0x64;

	//AiManager
	constexpr auto oAiManagerTargetPos = 0x10;
	constexpr auto oAiManagerIsMoving = 0x198;
	constexpr auto oAiManagerPassedWaypoints = 0x19C;
	constexpr auto oAiManagerNavBegin = 0x1A4;
	constexpr auto oAiManagerDashSpeed = 0x1D0;
	constexpr auto oAiManagerIsDashing = 0x1EC;
	constexpr auto oAiManagerNavEnd = 0x1FC;
	constexpr auto oAiManagerServerPos = 0x2BC;
	constexpr auto oAiManagerVelocity = 0x2C8;
}
#endif //!_OFFSETS_H_