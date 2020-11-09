#pragma once

#ifndef  _INITIALIZE_H_
#define _INITIALIZE_H_

#include "ObjectManager.h"
#include <list>
#include <string>


class Initialize
{
public:

	const std::list<std::string>ChampNames = {
		"Aatrox", "Ahri", "Akali", "Alistar", "Amumu", "Anivia", "Annie", "Aphelios"
		"Ashe", "Aurelion Sol", "AurelionSol","Azir", "Bard", "Blitzcrank", "Brand", "Braum", "Caitlyn",
		"Camille", "Cassiopeia", "Cho'Gath", "Chogath", "Corki", "Darius", "Diana", "Dr. Mundo", "Drmundo", "Draven",
		"Ekko", "Elise", "Evelynn", "Ezreal", "FiddleSticks", "Fiora", "Fizz", "Galio",
		"Gangplank", "Garen", "Gnar", "Gragas", "Graves", "Hecarim", "Heimerdinger", "Illaoi", "Irelia",
		"Ivern", "Janna", "Jarvan IV","JarvanIV", "Jax", "Jayce", "Jhin", "Jinx", "Kai'sa", "Kaisa", "Kalista", "Karma",
		"Karthus", "Kassadin", "Katarina", "Kayle", "Kayn", "Kennen", "Kha'zix", "Kindred", "Kled", "Kog'maw", "KogMaw",
		"LeBlanc", "Lee Sin", "Leona", "Lillia", "Lissandra", "Lucian", "Lulu", "Lux", "Malphite", "Malzahar", "Maokai",
		"Master Yi", "MasterYi", "Miss Fortune", "MissFortune", "Mordekaiser", "Morgana", "Nami", "Nasus", "Nautilus", "Neeko",
		"Nidalee", "Nocturne", "Nunu" , "Nunu & Willump", "Olaf", "Orianna", "Ornn", "Pantheon", "Poppy", "Pyke", "Qiyana", "Quinn",
		"Rakan", "Rammus", "Rek'Sai", "RekSai", "Renekton", "Rengar", "Riven", "Rumble", "Ryze", "Samira", "Sejuani", "Senna", "Seraphine", "Sett", "Shaco",
		"Shen", "Shyvana", "Singed", "Sion", "Sivir", "Skarner", "Sona", "Soraka", "Swain", "Sylas", "Syndra", "Tahm Kench", "TahmKench",
		"Taliyah", "Talon", "Taric", "Teemo", "Thresh", "Tristana", "Trundle", "Tryndamere", "Twisted Fate", "TwistedFate", "Twitch",
		"Udyr", "Urgot", "Varus", "Vayne", "Veigar", "Vel'Koz", "Velkoz","Vi", "Viktor", "Vladimir", "Volibear",
		"Warwick", "Wukong", "Xayah", "Xerath", "Xin Zhao", "XinZhao","Yasuo", "Yone", "Yorick", "Yuumi", "Zac", "Zed", "Ziggs", "Zilean", "Zoe", "Zyra",
		"MonkeyKing", "Mega Gnar", "MegaGnar", "Target Dummy", "TargetDummy"
	};

	//std::list<CObject>ObjList;
	std::list<CObject>herolist;
	std::list<CObject>turretlist;
	std::list<CObject>inhiblist;
	void AddObjects();
};

extern Initialize* init;

#endif // ! _INITIALIZE_H_