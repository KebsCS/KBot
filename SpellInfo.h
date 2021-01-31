#pragma once
#include <string>
#include <map>
#include "Definitions.h"

/// Flags of a spell/missile (they are the same thing anyway)
enum SpellFlags {
	// Flags from the game data files
	AffectAllyChampion = 1,
	AffectEnemyChampion = 1 << 1,
	AffectAllyLaneMinion = 1 << 2,
	AffectEnemyLaneMinion = 1 << 3,
	AffectAllyWard = 1 << 4,
	AffectEnemyWard = 1 << 5,
	AffectAllyTurret = 1 << 6,
	AffectEnemyTurret = 1 << 7,
	AffectAllyInhibs = 1 << 8,
	AffectEnemyInhibs = 1 << 9,
	AffectAllyNonLaneMinion = 1 << 10,
	AffectJungleMonster = 1 << 11,
	AffectEnemyNonLaneMinion = 1 << 12,
	AffectAlwaysSelf = 1 << 13,
	AffectNeverSelf = 1 << 14,

	// Custom flags set by us. These flags cant be unpacked from the game files (exception Targeted flag).
	ProjectedDestination = 1 << 22,

	AffectAllyMob = AffectAllyLaneMinion | AffectAllyNonLaneMinion,
	AffectEnemyMob = AffectEnemyLaneMinion | AffectEnemyNonLaneMinion | AffectJungleMonster,
	AffectAllyGeneric = AffectAllyMob | AffectAllyChampion,
	AffectEnemyGeneric = AffectEnemyMob | AffectEnemyChampion,
};

enum SpellType
{
	linear,
	polygon,
	circular,
	conic,
	rectangular,
	threeway,
};

/// Static data of a spell that we load from disk
class SpellInfo {
public:
	SpellInfo* AddFlags(SpellFlags flags);
public:
	// Values from game's data files
	std::string name;
	std::string icon;

	SpellFlags flags;
	float delay;
	float castRange;
	float castRadius;
	float width;
	float height;
	float speed;
	float travelTime;
};