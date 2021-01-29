#include "SpellInfo.h"

SpellInfo* SpellInfo::AddFlags(SpellFlags flags)
{
	this->flags = (SpellFlags)(this->flags | flags);
	return this;
}