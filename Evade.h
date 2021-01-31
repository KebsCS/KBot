#pragma once

#ifndef _EVADE_H_
#define _EVADE_H_

#include "Geometry.h"
#include "Menu.h"
#include "CObjectManager.h"
#include "Initialize.h"
#include "CSpellEntry.h"
#include "Mouse.h"
#include "Keyboard.h"

#define MathHuge 999999.f

class Evade
{
private:

	enum SSlot :int
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
		bool dodge = true;
	};

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

	struct EvadeSpellDB
	{
		std::string hero;
		std::vector<EvadeSpell>spells;
	};
	std::vector< EvadeSpellDB>evadespelldb;

	struct Champ
	{
		std::string hero;
		std::vector<Spell>spells;
	};

	std::vector<Champ>champs;

	struct DetectedSpell
	{
		Geometry::Polygon path;
		Geometry::Polygon path2;
		Vector3 startPos;
		Vector3 endPos;
		Spell spell;
		float startTime;
		Vector3 position;

		DetectedSpell(Geometry::Polygon& Path,
			Geometry::Polygon& Path2,
			Vector3& StartPos,
			Vector3& EndPos,
			Spell& Spell,
			float& StartTime
		)
		{
			path = Path;
			path2 = Path2;
			startPos = StartPos;
			endPos = EndPos;
			spell = Spell;
			startTime = StartTime;
			position = StartPos;
		}
	};

	std::vector<Geometry::Polygon> WorldMap;

	bool InWall(Vector3 p)
	{
		for (auto& poly : WorldMap)
		{
			if (poly.IsInside(p))
				return true;
		}
		return false;
	}

public:

	void MakeWorldMap()
	{
		//Vector3 mpos = mouse->GetWorldPos();

		//if (PressedKey(VK_XBUTTON2))
		//{
		//	std::string temp = ".Add(Vector3(" + std::to_string(mpos.x) + ", " + std::to_string(mpos.y) + ", " + std::to_string(mpos.z) + "));";
		//	//std::string temp = "Vector3(" + std::to_string(mpos.x) + ", " + std::to_string(mpos.y) + ", " + std::to_string(mpos.z) + ")";
		//	utils->CopyToClipboard(temp);
		//}

		//for (auto& poly : WorldMap)
		//{
		//	if (poly.IsInside(mpos))
		//		draw->Polygon(poly, RGBA(255, 0, 0));
		//	else
		//		draw->Polygon(poly, RGBA(255, 255, 255));
		//}

		std::vector<Geometry::Polygon>WorldMapPolygons;

		// blue bot side

		Geometry::Polygon BlueKrugsThickWall;
		BlueKrugsThickWall.Add(Vector3(7756.234863, 52.639771, 2464.992188));
		BlueKrugsThickWall.Add(Vector3(7640.35, 52.50, 2733.95));
		BlueKrugsThickWall.Add(Vector3(7672.863770, 52.585938, 3129.237549));
		BlueKrugsThickWall.Add(Vector3(8063.564453, 51.550659, 3155.882324));
		BlueKrugsThickWall.Add(Vector3(8129.348633, 51.130005, 2497.671875));
		WorldMapPolygons.emplace_back(BlueKrugsThickWall);

		Geometry::Polygon BlueKrugsLongWall;
		BlueKrugsLongWall.Add(Vector3(8127.571777, 52.639771, 2815.957520));
		BlueKrugsLongWall.Add(Vector3(8023.057129, 51.550781, 3166.459961));
		BlueKrugsLongWall.Add(Vector3(8961.963867, 54.594360, 3427.675025));
		BlueKrugsLongWall.Add(Vector3(10143.913086, 49.136353, 2958.775879));
		BlueKrugsLongWall.Add(Vector3(10287.332031, 49.222900, 2705.301758));
		BlueKrugsLongWall.Add(Vector3(9355.455078, 49.223022, 2830.78762));
		BlueKrugsLongWall.Add(Vector3(8949.976563, 49.222900, 2629.175781));
		BlueKrugsLongWall.Add(Vector3(8716.4, 49.3, 2594.2));
		BlueKrugsLongWall.Add(Vector3(8565.4, 50.9, 2857.26));
		WorldMapPolygons.emplace_back(BlueKrugsLongWall);

		Geometry::Polygon BlueKrugsFront;
		BlueKrugsFront.Add(Vector3(8241.81, 51.13, 2072.45));
		BlueKrugsFront.Add(Vector3(8916.39, 51.28, 2195.55));
		BlueKrugsFront.Add(Vector3(9126.26, 53.60, 1784.77));
		BlueKrugsFront.Add(Vector3(8226.55, 49.53, 1745.95));
		WorldMapPolygons.emplace_back(BlueKrugsFront);

		Geometry::Polygon BlueKrugsRight;
		BlueKrugsRight.Add(Vector3(9405.157227, 49.246338, 2380.270020));
		BlueKrugsRight.Add(Vector3(10608.626953, 49.231567, 2144.081543));
		BlueKrugsRight.Add(Vector3(10642.925781, 49.234131, 1811.047119));
		BlueKrugsRight.Add(Vector3(9598.166992, 53.245850, 1800.151855));
		WorldMapPolygons.emplace_back(BlueKrugsRight);

		Geometry::Polygon BlueBotT2;

		WorldMapPolygons.emplace_back(BlueBotT2);

		BlueBotT2.Add(Vector3(6435.320801, 49.556274, 1716.114014));
		BlueBotT2.Add(Vector3(6301.966309, 52.136108, 2148.675049));
		BlueBotT2.Add(Vector3(6486.218262, 52.149902, 2380.225586));
		BlueBotT2.Add(Vector3(7238.117188, 52.251831, 2436.216553));
		BlueBotT2.Add(Vector3(7615.313965, 51.885620, 2081.093750));
		BlueBotT2.Add(Vector3(7905.021973, 51.049561, 2005.945313));
		BlueBotT2.Add(Vector3(7876.043457, 49.644531, 1766.991455));;

		WorldMapPolygons.emplace_back(BlueBotT2);

		Geometry::Polygon BlueBase;
		BlueBase.Add(Vector3(3424.451172, 95.747925, 453.637085));
		BlueBase.Add(Vector3(3206.531006, 93.375854, 133.078979));
		BlueBase.Add(Vector3(2401.128906, 93.375732, 96.650269));
		BlueBase.Add(Vector3(212.869202, 182.726440, 226.813110));
		BlueBase.Add(Vector3(86.607300, 145.880371, 1239.083130));
		BlueBase.Add(Vector3(107.412781, 93.375732, 2322.179688));
		BlueBase.Add(Vector3(499.762329, 95.748169, 2347.572998));
		BlueBase.Add(Vector3(472.846619, 173.787476, 943.304504));
		BlueBase.Add(Vector3(1091.331787, 156.221680, 407.718323));
		BlueBase.Add(Vector3(3539.635010, 95.748047, 509.475159));

		WorldMapPolygons.emplace_back(BlueBase);

		Geometry::Polygon BlueBotWall0;
		BlueBotWall0.Add(Vector3(5058.401855, 19.594482, 634.870361));
		BlueBotWall0.Add(Vector3(4997.675781, 42.050293, 258.407471));
		BlueBotWall0.Add(Vector3(3268.790527, 93.375610, 206.596130));
		BlueBotWall0.Add(Vector3(3419.563477, 95.748047, 499.738556));
		WorldMapPolygons.emplace_back(BlueBotWall0);

		Geometry::Polygon BlueBotWall1;
		BlueBotWall1.Add(Vector3(4969.692383, 38.717651, 658.217163));
		BlueBotWall1.Add(Vector3(7113.463379, 49.449829, 686.936523));
		BlueBotWall1.Add(Vector3(7026.772949, 49.450439, 296.419739));
		BlueBotWall1.Add(Vector3(5033.240234, 30.646118, 306.336853));
		WorldMapPolygons.emplace_back(BlueBotWall1);

		Geometry::Polygon BlueBotWall2;
		BlueBotWall2.Add(Vector3(7116.141113, 49.449219, 644.408447));
		BlueBotWall2.Add(Vector3(8606.675781, 50.021240, 676.328491));
		BlueBotWall2.Add(Vector3(8872.407227, 50.289551, 389.215088));
		BlueBotWall2.Add(Vector3(7033.401367, 49.450562, 286.841064));
		WorldMapPolygons.emplace_back(BlueBotWall2);

		Geometry::Polygon BlueBotWall3;
		BlueBotWall3.Add(Vector3(8574.805664, 50.019043, 585.156067));
		BlueBotWall3.Add(Vector3(10807.851563, 49.868896, 741.032776));
		BlueBotWall3.Add(Vector3(10665.525391, 50.949463, 323.227478));
		BlueBotWall3.Add(Vector3(8498.418945, 50.010498, 377.790466));
		WorldMapPolygons.emplace_back(BlueBotWall3);

		Geometry::Polygon BlueBotWall4;
		BlueBotWall4.Add(Vector3(10747.920898, 49.763550, 711.136658));
		BlueBotWall4.Add(Vector3(12640.024414, 53.981445, 1316.020752));
		BlueBotWall4.Add(Vector3(13153.238281, 48.603271, 966.120544));
		BlueBotWall4.Add(Vector3(10653.873047, 48.681519, 179.386475));
		WorldMapPolygons.emplace_back(BlueBotWall4);

		Geometry::Polygon BotAlcove;
		BotAlcove.Add(Vector3(12758.837891, 52.981079, 1275.291138));
		BotAlcove.Add(Vector3(13266.686523, 44.839355, 991.961609));
		BotAlcove.Add(Vector3(13983.509766, 50.218018, 1792.741699));
		BotAlcove.Add(Vector3(13571.318359, 51.366943, 2325.767822));
		BotAlcove.Add(Vector3(13796.959961, 51.366821, 2753.065918));
		BotAlcove.Add(Vector3(14669.617188, 51.367065, 1696.142090));
		BotAlcove.Add(Vector3(13435.866211, 55.475708, 342.462585));
		BotAlcove.Add(Vector3(12412.002930, 51.271973, 608.571716));

		WorldMapPolygons.emplace_back(BotAlcove);

		Geometry::Polygon BotAlcoveSingle;
		BotAlcoveSingle.Add(Vector3(12914.253906, 52.106201, 1725.942627));
		BotAlcoveSingle.Add(Vector3(13266.424805, 51.326416, 2048.113037));
		BotAlcoveSingle.Add(Vector3(13520.624023, 33.823486, 1755.157227));
		BotAlcoveSingle.Add(Vector3(13181.631836, 33.836060, 1426.352295));

		WorldMapPolygons.emplace_back(BotAlcoveSingle);

		Geometry::Polygon BlueBotFirstJungleWall;
		BlueBotFirstJungleWall.Add(Vector3(5456.831543, 50.693970, 1784.110596));
		BlueBotFirstJungleWall.Add(Vector3(5444.199219, 51.277832, 3293.449707));
		BlueBotFirstJungleWall.Add(Vector3(5594.929199, 51.494873, 3443.072266));
		BlueBotFirstJungleWall.Add(Vector3(5871.554199, 51.339844, 3439.913086));
		BlueBotFirstJungleWall.Add(Vector3(6242.646973, 52.156128, 2794.958496));
		BlueBotFirstJungleWall.Add(Vector3(5734.213867, 51.799683, 2696.788086));
		BlueBotFirstJungleWall.Add(Vector3(5832.482422, 51.914795, 1979.982910));
		BlueBotFirstJungleWall.Add(Vector3(5941.416992, 50.717407, 1800.996826));

		WorldMapPolygons.emplace_back(BlueBotFirstJungleWall);

		Geometry::Polygon BlueBotGate;
		BlueBotGate.Add(Vector3(4314.375000, 95.296997, 3679.400391));
		BlueBotGate.Add(Vector3(4510.436035, 64.784546, 3845.580811));
		BlueBotGate.Add(Vector3(4875.211914, 56.912109, 3033.028076));
		BlueBotGate.Add(Vector3(4861.744629, 58.027222, 3031.735352));

		BlueBotGate.Add(Vector3(4979.687500, 51.705566, 2613.983887));
		BlueBotGate.Add(Vector3(5036.854004, 47.544678, 1783.889893));
		BlueBotGate.Add(Vector3(4754.667969, 95.748169, 1743.148438));
		BlueBotGate.Add(Vector3(4729.971191, 93.924927, 2641.882568));
		BlueBotGate.Add(Vector3(4628.165527, 96.021118, 2965.928223));

		WorldMapPolygons.emplace_back(BlueBotGate);

		Geometry::Polygon BlueMidT2;
		BlueMidT2.Add(Vector3(5288.692871, 50.815308, 3788.989746));
		BlueMidT2.Add(Vector3(4856.758301, 50.329468, 4273.816895));
		BlueMidT2.Add(Vector3(5635.197754, 47.306641, 4916.609863));
		BlueMidT2.Add(Vector3(6099.122559, 48.507202, 4797.158203));
		BlueMidT2.Add(Vector3(5629.668945, 51.383301, 3771.138428));

		WorldMapPolygons.emplace_back(BlueMidT2);

		Geometry::Polygon BlueRaptors;

		BlueRaptors.Add(Vector3(5896.820801, 51.787109, 5351.881836));
		BlueRaptors.Add(Vector3(6804.387207, 52.069702, 6208.641602));
		BlueRaptors.Add(Vector3(7677.117676, 52.805542, 5588.629395));
		BlueRaptors.Add(Vector3(7699.606445, 48.662354, 5272.340820));
		BlueRaptors.Add(Vector3(7231.230469, 48.648682, 5266.850586));
		BlueRaptors.Add(Vector3(7129.101074, 56.685425, 5545.067383));
		BlueRaptors.Add(Vector3(6878.145508, 58.155151, 5618.133789));
		BlueRaptors.Add(Vector3(6746.679688, 52.410645, 5444.405762));
		BlueRaptors.Add(Vector3(6724.510254, 48.526978, 4826.553223));
		BlueRaptors.Add(Vector3(6451.996582, 48.527344, 4862.534180));
		BlueRaptors.Add(Vector3(6395.839355, 48.527344, 5314.148926));
		BlueRaptors.Add(Vector3(6008.504395, 49.564087, 5208.062012));

		WorldMapPolygons.emplace_back(BlueRaptors);

		Geometry::Polygon BlueBotCenter;
		BlueBotCenter.Add(Vector3(6155.737305, 49.746948, 3912.269287));
		BlueBotCenter.Add(Vector3(6468.141113, 48.527832, 4539.146973));
		BlueBotCenter.Add(Vector3(6637.203125, 48.526733, 4477.838867));
		BlueBotCenter.Add(Vector3(6653.528320, 48.515137, 3845.246582));
		BlueBotCenter.Add(Vector3(6735.877930, 49.375854, 3751.117920));
		BlueBotCenter.Add(Vector3(7063.973633, 52.542847, 3622.645508));
		BlueBotCenter.Add(Vector3(7154.296875, 52.505249, 3216.163086));
		BlueBotCenter.Add(Vector3(6825.978027, 51.203369, 3160.357910));
		BlueBotCenter.Add(Vector3(6598.780273, 49.582886, 3222.549561));

		WorldMapPolygons.emplace_back(BlueBotCenter);

		Geometry::Polygon BotRedBuff;
		BotRedBuff.Add(Vector3(7132.430176, 48.523560, 4109.831055));
		BotRedBuff.Add(Vector3(7125.850586, 48.527222, 4548.585449));
		BotRedBuff.Add(Vector3(8241.053711, 51.334106, 4740.954102));
		BotRedBuff.Add(Vector3(8552.422852, 51.407349, 4674.666016));
		BotRedBuff.Add(Vector3(8650.676758, 52.184326, 4504.285645));
		BotRedBuff.Add(Vector3(8512.589844, 53.421753, 3856.878662));
		BotRedBuff.Add(Vector3(8315.083008, 54.110474, 3615.036133));
		BotRedBuff.Add(Vector3(7922.908691, 51.968506, 3559.327637));
		BotRedBuff.Add(Vector3(7853.631836, 52.684814, 3676.190674));
		BotRedBuff.Add(Vector3(8092.403809, 53.721191, 3948.928467));
		BotRedBuff.Add(Vector3(7917.744141, 53.718994, 4319.865234));
		BotRedBuff.Add(Vector3(7451.589844, 54.677368, 4034.372559));
		BotRedBuff.Add(Vector3(7233.829102, 50.696533, 3998.119873));

		WorldMapPolygons.emplace_back(BotRedBuff);

		Geometry::Polygon BlueBotTriBush;
		BlueBotTriBush.Add(Vector3(10616.373047, 48.478027, 3108.396973));
		BlueBotTriBush.Add(Vector3(11024.078125, -48.062500, 3310.496094));
		BlueBotTriBush.Add(Vector3(11785.628906, 16.665283, 2728.390137));
		BlueBotTriBush.Add(Vector3(11792.351563, 52.181885, 2534.448242));
		BlueBotTriBush.Add(Vector3(11457.856445, 49.246704, 1970.302734));
		BlueBotTriBush.Add(Vector3(11201.701172, 49.244507, 1914.936768));
		BlueBotTriBush.Add(Vector3(10993.331055, 49.222778, 2495.226563));

		WorldMapPolygons.emplace_back(BlueBotTriBush);

		Geometry::Polygon Drake;

		Drake.Add(Vector3(8680.609375, 52.032715, 5158.563965));
		Drake.Add(Vector3(8759.239258, -45.179443, 5411.269531));
		Drake.Add(Vector3(8971.355469, -71.252441, 5441.652344));
		Drake.Add(Vector3(9994.673828, -71.009399, 5038.745117));
		Drake.Add(Vector3(9967.694336, -71.240723, 4927.219238));
		Drake.Add(Vector3(9506.804688, -71.240479, 4851.296875));
		Drake.Add(Vector3(9331.930664, -71.240723, 4595.021484));
		Drake.Add(Vector3(9358.117188, -62.019531, 4216.667969));
		Drake.Add(Vector3(9764.786133, -71.240601, 3854.887695));
		Drake.Add(Vector3(10182.078125, -71.240601, 3968.331055));
		Drake.Add(Vector3(10405.980469, -71.240601, 4477.082520));
		Drake.Add(Vector3(10315.000977, -71.240601, 4684.463379));
		Drake.Add(Vector3(10402.255859, -70.638794, 4761.433105));
		Drake.Add(Vector3(10957.761719, -71.240601, 4135.506836));
		Drake.Add(Vector3(10934.603516, -37.828491, 3741.134277));
		Drake.Add(Vector3(10495.129883, 48.207153, 3646.969238));
		Drake.Add(Vector3(10365.166016, 49.112549, 3243.314453));
		Drake.Add(Vector3(10129.605469, 49.221680, 3302.135498));
		Drake.Add(Vector3(9500.646484, 66.103394, 3606.070313));
		Drake.Add(Vector3(9118.067383, 53.979614, 4034.264404));
		Drake.Add(Vector3(9041.540039, 52.427246, 4495.880371));
		Drake.Add(Vector3(9091.586914, 51.434692, 4821.541016));

		WorldMapPolygons.emplace_back(Drake);

		Geometry::Polygon MidSingleWall1;
		MidSingleWall1.Add(Vector3(7231.703125, 52.043823, 6582.380371));
		MidSingleWall1.Add(Vector3(7396.997559, 52.251709, 6715.939941));
		MidSingleWall1.Add(Vector3(8514.631836, -70.367310, 5872.724121));
		MidSingleWall1.Add(Vector3(8423.047852, -35.424194, 5753.101563));

		WorldMapPolygons.emplace_back(MidSingleWall1);

		Geometry::Polygon MidSingleWall2;
		MidSingleWall2.Add(Vector3(8254.245117, 51.136841, 7233.436523));
		MidSingleWall2.Add(Vector3(8558.350586, 52.540283, 7550.896973));
		MidSingleWall2.Add(Vector3(9962.541016, 51.785156, 6785.833984));
		MidSingleWall2.Add(Vector3(9603.333984, -54.624146, 6325.055176));

		WorldMapPolygons.emplace_back(MidSingleWall2);

		Geometry::Polygon MidSingleWall3;
		MidSingleWall3.Add(Vector3(6373.239746, 51.812500, 7198.722656));
		MidSingleWall3.Add(Vector3(5451.577148, 51.682739, 8000.904785));
		MidSingleWall3.Add(Vector3(4848.746582, 51.390625, 8028.944824));
		MidSingleWall3.Add(Vector3(5249.678711, -69.630615, 8654.394531));
		MidSingleWall3.Add(Vector3(6674.899902, 52.293457, 7441.341797));

		WorldMapPolygons.emplace_back(MidSingleWall3);

		Geometry::Polygon MidSingleWall4;
		MidSingleWall4.Add(Vector3(7363.403809, 49.694946, 8018.789551));
		MidSingleWall4.Add(Vector3(6272.020508, -71.240479, 9082.463867));
		MidSingleWall4.Add(Vector3(6499.270020, 1.544922, 9175.778320));
		MidSingleWall4.Add(Vector3(7609.894531, 52.537720, 8190.222168));

		WorldMapPolygons.emplace_back(MidSingleWall4);

		Geometry::Polygon BlueBaseRock1;
		BlueBaseRock1.Add(Vector3(3875.906494, 95.748047, 2282.250488));
		BlueBaseRock1.Add(Vector3(4167.996094, 95.747925, 2283.832275));
		BlueBaseRock1.Add(Vector3(4104.306641, 95.748047, 2544.140625));
		BlueBaseRock1.Add(Vector3(3830.846680, 95.748047, 2430.804688));

		WorldMapPolygons.emplace_back(BlueBaseRock1);

		Geometry::Polygon BlueBaseRock2;
		BlueBaseRock2.Add(Vector3(2493.458252, 95.747925, 3875.603027));
		BlueBaseRock2.Add(Vector3(2297.197754, 95.748047, 3949.369141));
		BlueBaseRock2.Add(Vector3(2312.656250, 95.747803, 4243.294922));
		BlueBaseRock2.Add(Vector3(2593.144531, 95.748047, 4155.326660));

		WorldMapPolygons.emplace_back(BlueBaseRock2);

		//blue bot turrets
		float turretr = 130;
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(2156.299072, 95.748291, 1864.703125), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(4265.898926, 95.748047, 1299.033203), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(6908.365234, 49.448364, 1530.404297), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(10484.264648, 50.589722, 1034.449829), turretr).ToPolygon());

		//blue mid turrets
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(3632.052490, 95.748169, 3703.805664), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(5043.881836, 50.257202, 4819.460449), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(5852.820313, 51.796143, 6421.986816), turretr).ToPolygon());

		//blue top turrets
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(1729.542236, 95.747925, 2294.603516), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(1166.900391, 95.747559, 4299.940430), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(1499.213257, 52.838257, 6703.171875), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(946.924011, 52.838135, 10456.309570), turretr).ToPolygon());

		float inhibr = 200;
		//blue inhibs
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(3454.257080, 93.375610, 1238.949219), inhibr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(3251.918701, 93.376099, 3244.858154), inhibr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(1164.845581, 93.375732, 3600.786133), inhibr).ToPolygon());

		float nexusr = 310;
		//blue nexus
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(1553.457764, 93.375854, 1675.431396), nexusr).ToPolygon());

		//red bot turrets
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13055.509766, 93.336426, 12615.729492), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13627.459961, 93.336060, 10578.129883), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13327.817383, 52.306519, 8221.876953), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13871.083008, 52.940308, 4504.009766), turretr).ToPolygon());

		//red mid turrets
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(11124.986328, 93.336426, 11211.146484), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(9764.289063, 52.305054, 10123.541992), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(8960.620117, 54.014648, 8509.208984), turretr).ToPolygon());

		//red top turrets
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(12611.103516, 93.336670, 13090.678711), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(10475.872070, 95.580811, 13661.163086), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(7943.056641, 52.838257, 13411.973633), turretr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(4314.484863, 52.838257, 13891.267578), turretr).ToPolygon());

		//red inhibs
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13608.256836, 93.338257, 11311.201172), inhibr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(11588.635742, 93.336792, 11692.488281), inhibr).ToPolygon());
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(11267.033203, 93.337280, 13690.755859), inhibr).ToPolygon());

		//red nexus
		WorldMapPolygons.emplace_back(Geometry::Circle(Vector3(13260.798828, 93.338013, 13253.446289), nexusr).ToPolygon());

		//red bot side

		Geometry::Polygon RedBaseRock1;

		RedBaseRock1.Add(Vector3(12491.235352, 91.430054, 10787.177734));
		RedBaseRock1.Add(Vector3(12547.904297, 91.430054, 11084.752930));
		RedBaseRock1.Add(Vector3(12311.783203, 91.430176, 11173.816406));
		RedBaseRock1.Add(Vector3(12272.307617, 91.429688, 10829.805664));
		WorldMapPolygons.emplace_back(RedBaseRock1);

		Geometry::Polygon RedBaseRock2;

		RedBaseRock2.Add(Vector3(10787.097656, 91.429932, 12411.529297));
		RedBaseRock2.Add(Vector3(10734.474609, 91.429688, 12654.679688));
		RedBaseRock2.Add(Vector3(11012.180664, 91.429688, 12654.679688));
		RedBaseRock2.Add(Vector3(11060.509766, 91.430054, 12431.608398));
		WorldMapPolygons.emplace_back(RedBaseRock2);

		Geometry::Polygon RedBotWall;
		RedBotWall.Add(Vector3(12582.789063, 51.366821, 3252.490723));
		RedBotWall.Add(Vector3(12220.952148, -7.825684, 3384.810059));
		RedBotWall.Add(Vector3(12025.471680, -66.247192, 3691.102051));
		RedBotWall.Add(Vector3(11926.722656, -71.240845, 4272.294922));
		RedBotWall.Add(Vector3(11407.234375, -71.240601, 4969.239258));
		RedBotWall.Add(Vector3(11320.976563, -70.561279, 5130.328125));
		RedBotWall.Add(Vector3(11486.030273, 48.295776, 5676.755859));
		RedBotWall.Add(Vector3(11603.736328, 54.895508, 5568.621094));
		RedBotWall.Add(Vector3(11593.289063, 51.779053, 5195.991699));
		RedBotWall.Add(Vector3(12129.412109, 51.450684, 4483.300781));
		RedBotWall.Add(Vector3(12500.642578, 51.729370, 4491.774414));
		RedBotWall.Add(Vector3(12655.236328, 51.729492, 4907.925293));
		RedBotWall.Add(Vector3(12898.687500, 51.729248, 5113.024414));
		RedBotWall.Add(Vector3(13058.929688, 51.729736, 5087.220703));
		RedBotWall.Add(Vector3(13031.500977, 51.367065, 3976.888184));
		RedBotWall.Add(Vector3(12827.536133, 51.366943, 3366.615723));

		WorldMapPolygons.emplace_back(RedBotWall);

		Geometry::Polygon RedGromp;
		RedGromp.Add(Vector3(13080.004883, 51.736450, 5492.735352));
		RedGromp.Add(Vector3(12649.204102, 51.689697, 5494.173340));
		RedGromp.Add(Vector3(12810.389648, 51.649902, 6506.116211));
		RedGromp.Add(Vector3(12104.416016, 51.707031, 6918.362305));
		RedGromp.Add(Vector3(12093.443359, 51.949341, 7232.392090));
		RedGromp.Add(Vector3(12256.777344, 51.922607, 7246.396484));
		RedGromp.Add(Vector3(13098.047852, 52.727539, 6773.684570));
		RedGromp.Add(Vector3(13134.636719, 55.501465, 5974.168945));

		WorldMapPolygons.emplace_back(RedGromp);

		Geometry::Polygon RedBlueBuff;
		RedBlueBuff.Add(Vector3(10847.879883, -64.234497, 5574.210938));
		RedBlueBuff.Add(Vector3(10040.683594, -44.869995, 6135.292969));
		RedBlueBuff.Add(Vector3(10454.040039, 54.333740, 6443.572266));
		RedBlueBuff.Add(Vector3(10394.473633, 51.968262, 6939.541016));
		RedBlueBuff.Add(Vector3(10254.172852, 51.959961, 7245.502441));
		RedBlueBuff.Add(Vector3(10484.497070, 51.696045, 7447.589844));
		RedBlueBuff.Add(Vector3(11046.335938, 52.203979, 7467.188477));
		RedBlueBuff.Add(Vector3(11697.986328, 51.851074, 7364.248535));
		RedBlueBuff.Add(Vector3(11669.078125, 51.726440, 7165.153809));
		RedBlueBuff.Add(Vector3(10864.930664, 51.722656, 7267.061523));
		RedBlueBuff.Add(Vector3(10700.602539, 51.722900, 7132.901367));
		RedBlueBuff.Add(Vector3(10950.834961, 51.705078, 6744.607910));
		RedBlueBuff.Add(Vector3(11022.414063, 51.024414, 6330.088379));
		RedBlueBuff.Add(Vector3(11128.287109, 49.696045, 6018.697266));
		RedBlueBuff.Add(Vector3(11166.317383, 41.913940, 5856.528809));
		RedBlueBuff.Add(Vector3(11180.398438, 37.859497, 5805.675293));
		WorldMapPolygons.emplace_back(RedBlueBuff);

		Geometry::Polygon RedWolves;
		RedWolves.Add(Vector3(10826.686523, 63.062378, 8640.513672));
		RedWolves.Add(Vector3(10777.289063, 63.238892, 8303.173828));
		RedWolves.Add(Vector3(10959.771484, 62.847412, 8102.148438));
		RedWolves.Add(Vector3(11345.247070, 55.702759, 8211.702148));
		RedWolves.Add(Vector3(11394.072266, 52.265625, 8024.532227));
		RedWolves.Add(Vector3(10943.361328, 52.147949, 7784.086914));
		RedWolves.Add(Vector3(10452.662109, 51.666748, 7859.534668));
		RedWolves.Add(Vector3(10299.335938, 52.010620, 8132.997070));
		RedWolves.Add(Vector3(10432.843750, 63.811401, 8591.557617));
		RedWolves.Add(Vector3(10660.953125, 62.780029, 8733.992188));

		WorldMapPolygons.emplace_back(RedWolves);

		Geometry::Polygon RedMidT1;
		RedMidT1.Add(Vector3(8917.677734, 51.664673, 8019.327637));
		RedMidT1.Add(Vector3(9687.733398, 53.013062, 8805.222656));
		RedMidT1.Add(Vector3(10078.872070, 51.949341, 8672.130859));
		RedMidT1.Add(Vector3(9861.271484, 61.385376, 8205.104492));
		RedMidT1.Add(Vector3(9941.381836, 51.728760, 7628.883789));
		RedMidT1.Add(Vector3(9886.410156, 51.166504, 7455.178711));
		RedMidT1.Add(Vector3(9482.649414, 52.791992, 7415.209473));
		WorldMapPolygons.emplace_back(RedMidT1);

		Geometry::Polygon RedBotSingle;
		RedBotSingle.Add(Vector3(11535.732422, 51.674805, 6307.197754));
		RedBotSingle.Add(Vector3(11747.701172, 51.726074, 6552.771484));
		RedBotSingle.Add(Vector3(12310.318359, 56.996948, 6267.325684));
		RedBotSingle.Add(Vector3(12301.819336, 57.270996, 5785.022461));
		RedBotSingle.Add(Vector3(12282.183594, 51.728882, 5133.541992));
		RedBotSingle.Add(Vector3(12111.533203, 53.258545, 5132.801758));
		RedBotSingle.Add(Vector3(11991.067383, 50.338745, 5798.843750));
		RedBotSingle.Add(Vector3(11537.567383, 51.401245, 6251.696289));

		WorldMapPolygons.emplace_back(RedBotSingle);

		Geometry::Polygon RedBotT2;
		RedBotT2.Add(Vector3(12439.131836, 51.666138, 7674.786133));
		RedBotT2.Add(Vector3(12415.541992, 52.311646, 8368.718750));
		RedBotT2.Add(Vector3(12483.635742, 52.313110, 8572.959961));
		RedBotT2.Add(Vector3(12251.107422, 50.962280, 9073.270508));
		RedBotT2.Add(Vector3(12370.631836, 52.306519, 9371.778320));
		RedBotT2.Add(Vector3(13106.421875, 52.306396, 9331.301758));
		RedBotT2.Add(Vector3(13034.715820, 52.179321, 7363.865723));
		RedBotT2.Add(Vector3(12480.872070, 51.667480, 7619.645020));

		WorldMapPolygons.emplace_back(RedBotT2);

		Geometry::Polygon RedBotFirstWall;
		RedBotFirstWall.Add(Vector3(9988.615234, 51.324707, 9103.666016));
		RedBotFirstWall.Add(Vector3(10637.549805, 52.285645, 9860.672852));
		RedBotFirstWall.Add(Vector3(11837.274414, 52.306274, 9431.736328));
		RedBotFirstWall.Add(Vector3(11730.719727, 50.307861, 8930.359375));
		RedBotFirstWall.Add(Vector3(12021.347656, 51.694336, 8675.562500));
		RedBotFirstWall.Add(Vector3(12020.026367, 52.570313, 7823.440430));
		RedBotFirstWall.Add(Vector3(11833.503906, 52.086548, 7815.117188));
		RedBotFirstWall.Add(Vector3(11774.011719, 52.865845, 7971.698730));
		RedBotFirstWall.Add(Vector3(11699.738281, 59.842407, 8591.692383));
		RedBotFirstWall.Add(Vector3(11387.248047, 66.840820, 8961.541016));
		RedBotFirstWall.Add(Vector3(10709.952148, 66.376343, 9212.975586));
		RedBotFirstWall.Add(Vector3(10027.418945, 51.120972, 9056.192383));

		WorldMapPolygons.emplace_back(RedBotFirstWall);

		Geometry::Polygon RedBotGate;
		RedBotGate.Add(Vector3(10973.461914, 84.186890, 10373.744141));
		RedBotGate.Add(Vector3(11141.634766, 93.530396, 10643.260742));
		RedBotGate.Add(Vector3(11946.921875, 91.429810, 10345.117188));
		RedBotGate.Add(Vector3(12361.036133, 91.429688, 10256.828125));
		RedBotGate.Add(Vector3(13210.646484, 98.236694, 10181.263672));
		RedBotGate.Add(Vector3(13154.001953, 61.686890, 9878.412109));
		RedBotGate.Add(Vector3(12336.843750, 52.306641, 9937.572266));
		RedBotGate.Add(Vector3(11842.802734, 56.026001, 10046.561523));
		RedBotGate.Add(Vector3(11162.035156, 52.306152, 10222.242188));
		WorldMapPolygons.emplace_back(RedBotGate);

		Geometry::Polygon RedTopGate;
		RedTopGate.Add(Vector3(10266.684570, 55.751465, 11096.410156));
		RedTopGate.Add(Vector3(9910.756836, 58.198242, 12012.918945));
		RedTopGate.Add(Vector3(9781.018555, 52.306274, 12391.927734));
		RedTopGate.Add(Vector3(9726.264648, 100.804199, 13289.105469));
		RedTopGate.Add(Vector3(10048.833984, 101.509155, 13266.652344));
		RedTopGate.Add(Vector3(10118.925781, 91.430176, 12343.614258));
		RedTopGate.Add(Vector3(10199.730469, 91.429565, 11971.855469));
		RedTopGate.Add(Vector3(10521.806641, 94.626831, 11190.311523));
		RedTopGate.Add(Vector3(10464.315430, 98.269043, 11089.249023));
		WorldMapPolygons.emplace_back(RedTopGate);

		Geometry::Polygon RedBotWall1;
		RedBotWall1.Add(Vector3(13602.685547, 51.366821, 2496.691650));
		RedBotWall1.Add(Vector3(14203.433594, 52.343750, 4116.619629));
		RedBotWall1.Add(Vector3(14605.052734, 51.862427, 3924.003418));
		RedBotWall1.Add(Vector3(14051.223633, 51.366821, 2343.111328));

		WorldMapPolygons.emplace_back(RedBotWall1);

		Geometry::Polygon RedBotWall2;
		RedBotWall2.Add(Vector3(14195.674805, 52.024414, 4051.196045));
		RedBotWall2.Add(Vector3(14200.820313, 52.852173, 5767.568359));
		RedBotWall2.Add(Vector3(14583.228516, 53.287109, 5710.626465));
		RedBotWall2.Add(Vector3(14579.732422, 51.666748, 3889.701172));
		WorldMapPolygons.emplace_back(RedBotWall2);

		Geometry::Polygon RedBotWall3;
		RedBotWall3.Add(Vector3(14203.834961, 52.861572, 5723.279297));
		RedBotWall3.Add(Vector3(14180.451172, 52.306274, 7293.068848));
		RedBotWall3.Add(Vector3(14645.187500, 52.306152, 7416.300781));
		RedBotWall3.Add(Vector3(14575.836914, 53.325439, 5600.418457));
		WorldMapPolygons.emplace_back(RedBotWall3);

		Geometry::Polygon RedBotWall4;
		RedBotWall4.Add(Vector3(14194.120117, 52.306152, 7156.503906));
		RedBotWall4.Add(Vector3(14133.347656, 52.306152, 9103.853516));
		RedBotWall4.Add(Vector3(14629.777344, 52.306274, 9153.485352));
		RedBotWall4.Add(Vector3(14656.063477, 52.306274, 7172.025879));
		WorldMapPolygons.emplace_back(RedBotWall4);

		Geometry::Polygon RedBotWall5;
		RedBotWall5.Add(Vector3(14127.816406, 52.306274, 9113.654297));
		RedBotWall5.Add(Vector3(14127.050781, 98.793457, 10117.333984));
		RedBotWall5.Add(Vector3(14609.739258, 94.635986, 9996.030273));
		RedBotWall5.Add(Vector3(14569.860352, 52.306519, 9060.099609));
		WorldMapPolygons.emplace_back(RedBotWall5);

		Geometry::Polygon RedBotWall6;
		RedBotWall6.Add(Vector3(14166.157227, 100.163574, 10003.862305));
		RedBotWall6.Add(Vector3(14330.084961, 91.429810, 11385.801758));
		RedBotWall6.Add(Vector3(14592.604492, 93.335938, 11447.001953));
		RedBotWall6.Add(Vector3(14561.793945, 36.608276, 9780.140625));
		WorldMapPolygons.emplace_back(RedBotWall6);

		Geometry::Polygon RedBotWall7;
		RedBotWall7.Add(Vector3(14346.009766, 91.429810, 11264.744141));
		RedBotWall7.Add(Vector3(14376.547852, 91.429688, 12866.640625));
		RedBotWall7.Add(Vector3(14635.976563, 93.335205, 12843.353516));
		RedBotWall7.Add(Vector3(14582.112305, 93.336182, 11265.173828));
		WorldMapPolygons.emplace_back(RedBotWall7);

		Geometry::Polygon RedBase;
		RedBase.Add(Vector3(14359.389648, 91.429932, 12850.298828));
		RedBase.Add(Vector3(14391.688477, 167.152954, 13949.299805));
		RedBase.Add(Vector3(13800.090820, 162.647095, 14427.491211));
		RedBase.Add(Vector3(13284.894531, 109.663818, 14344.791016));
		RedBase.Add(Vector3(12513.450195, 91.429810, 14363.634766));
		RedBase.Add(Vector3(12043.112305, 93.336426, 14682.115234));
		RedBase.Add(Vector3(14010.169922, 171.977661, 14591.741211));
		RedBase.Add(Vector3(14625.817383, 171.977783, 14056.609375));
		RedBase.Add(Vector3(14623.311523, 126.637451, 13540.045898));
		RedBase.Add(Vector3(14651.800781, 93.334473, 12785.875000));

		WorldMapPolygons.emplace_back(RedBase);

		//red top side

		Geometry::Polygon RedTopFirstWall;
		RedTopFirstWall.Add(Vector3(8985.766602, 56.090332, 11441.448242));
		RedTopFirstWall.Add(Vector3(8617.993164, 56.438721, 12074.437500));
		RedTopFirstWall.Add(Vector3(8960.138672, 56.476807, 12316.811523));
		RedTopFirstWall.Add(Vector3(8937.092773, 52.837891, 13116.291992));
		RedTopFirstWall.Add(Vector3(9374.091797, 52.789429, 13108.270508));
		RedTopFirstWall.Add(Vector3(9399.981445, 52.306274, 12016.232422));
		RedTopFirstWall.Add(Vector3(9363.283203, 52.306274, 11680.943359));
		RedTopFirstWall.Add(Vector3(9295.398438, 54.095093, 11473.519531));
		RedTopFirstWall.Add(Vector3(9063.618164, 54.596436, 11473.291992));
		WorldMapPolygons.emplace_back(RedTopFirstWall);

		Geometry::Polygon RedTopT2;
		RedTopT2.Add(Vector3(6941.366211, 52.838623, 13225.461914));
		RedTopT2.Add(Vector3(8424.693359, 53.558350, 13088.464844));
		RedTopT2.Add(Vector3(8595.156250, 56.476807, 12750.659180));
		RedTopT2.Add(Vector3(8562.202148, 56.476807, 12580.449219));
		RedTopT2.Add(Vector3(8246.428711, 56.476929, 12405.218750));
		RedTopT2.Add(Vector3(7742.108398, 56.476807, 12366.475586));
		RedTopT2.Add(Vector3(7427.307617, 56.476440, 12687.617188));
		RedTopT2.Add(Vector3(6953.705078, 56.173828, 12912.740234));
		RedTopT2.Add(Vector3(6942.652832, 53.975342, 13081.690430));

		WorldMapPolygons.emplace_back(RedTopT2);

		Geometry::Polygon RedTopMidT2;
		RedTopMidT2.Add(Vector3(9567.800781, 52.149902, 11122.319336));
		RedTopMidT2.Add(Vector3(9990.819336, 52.306274, 10587.238281));
		RedTopMidT2.Add(Vector3(9248.355469, 50.101807, 10002.863281));
		RedTopMidT2.Add(Vector3(9015.233398, 48.789429, 10027.662109));
		RedTopMidT2.Add(Vector3(8827.837891, 50.032227, 10016.448242));
		RedTopMidT2.Add(Vector3(8751.576172, 50.422241, 10154.259766));
		RedTopMidT2.Add(Vector3(8831.836914, 50.513306, 10385.578125));
		RedTopMidT2.Add(Vector3(9005.146484, 50.524414, 10621.493164));
		RedTopMidT2.Add(Vector3(9140.303711, 50.620239, 11116.252930));
		RedTopMidT2.Add(Vector3(9457.965820, 51.808105, 11145.666992));
		WorldMapPolygons.emplace_back(RedTopMidT2);

		Geometry::Polygon RedRaptors;
		RedRaptors.Add(Vector3(7988.451660, 52.799072, 8742.781250));
		RedRaptors.Add(Vector3(7164.697754, 52.867065, 9315.524414));
		RedRaptors.Add(Vector3(7214.285156, 54.398560, 9599.203125));
		RedRaptors.Add(Vector3(7625.979980, 52.450317, 9643.475586));
		RedRaptors.Add(Vector3(7648.810547, 52.480957, 9276.791992));
		RedRaptors.Add(Vector3(7971.102539, 52.513794, 9222.250000));
		RedRaptors.Add(Vector3(8080.961914, 52.296143, 9549.732422));
		RedRaptors.Add(Vector3(8133.667969, 49.299927, 10049.491211));
		RedRaptors.Add(Vector3(8364.844727, 50.381104, 10098.639648));
		RedRaptors.Add(Vector3(8373.485352, 50.384033, 9637.213867));
		RedRaptors.Add(Vector3(8954.506836, 50.465454, 9607.375000));
		RedRaptors.Add(Vector3(8949.221680, 51.252441, 9513.681641));
		RedRaptors.Add(Vector3(8037.153320, 52.891479, 8738.394531));

		WorldMapPolygons.emplace_back(RedRaptors);

		Geometry::Polygon RedRedBuff;
		RedRedBuff.Add(Vector3(6312.855957, 53.773804, 10153.846680));
		RedRedBuff.Add(Vector3(6173.232910, 54.802246, 10555.171875));
		RedRedBuff.Add(Vector3(6297.729004, 55.192505, 10915.541016));
		RedRedBuff.Add(Vector3(6797.297852, 53.829590, 11430.987305));
		RedRedBuff.Add(Vector3(7030.684570, 55.031738, 11239.633789));
		RedRedBuff.Add(Vector3(6788.081055, 56.001465, 10971.418945));
		RedRedBuff.Add(Vector3(6798.494141, 55.999878, 10646.976563));
		RedRedBuff.Add(Vector3(7416.108398, 55.709106, 10678.771484));
		RedRedBuff.Add(Vector3(7499.697266, 55.754761, 10934.747070));
		RedRedBuff.Add(Vector3(7732.304688, 51.677979, 10931.976563));
		RedRedBuff.Add(Vector3(7759.284180, 50.746704, 10395.048828));
		RedRedBuff.Add(Vector3(7083.908203, 52.650391, 10158.342773));
		RedRedBuff.Add(Vector3(6456.756836, 53.624023, 10108.638672));

		WorldMapPolygons.emplace_back(RedRedBuff);

		Geometry::Polygon RedRedBuffFront;
		RedRedBuffFront.Add(Vector3(7693.833008, 52.570068, 11733.673828));
		RedRedBuffFront.Add(Vector3(8269.440430, 56.476685, 11698.101563));
		RedRedBuffFront.Add(Vector3(8556.704102, 51.859985, 11248.744141));
		RedRedBuffFront.Add(Vector3(8699.910156, 50.510254, 10932.789063));
		RedRedBuffFront.Add(Vector3(8446.058594, 50.458374, 10422.591797));
		RedRedBuffFront.Add(Vector3(8200.726563, 49.260986, 10442.807617));
		RedRedBuffFront.Add(Vector3(8176.613281, 49.837524, 10713.337891));
		RedRedBuffFront.Add(Vector3(8189.938477, 50.624634, 11063.110352));
		RedRedBuffFront.Add(Vector3(7810.592773, 54.246460, 11308.677734));
		RedRedBuffFront.Add(Vector3(7683.240234, 51.266968, 11416.046875));
		RedRedBuffFront.Add(Vector3(7656.525879, 49.992676, 11593.346680));
		WorldMapPolygons.emplace_back(RedRedBuffFront);

		Geometry::Polygon RedGolems;
		RedGolems.Add(Vector3(7208.128906, 52.095825, 11687.616211));
		RedGolems.Add(Vector3(6158.486328, 56.908447, 11637.940430));
		RedGolems.Add(Vector3(6036.783203, 56.743774, 11483.250000));
		RedGolems.Add(Vector3(5802.975098, 56.875854, 11531.875000));
		RedGolems.Add(Vector3(5649.792969, 56.848389, 11638.380859));
		RedGolems.Add(Vector3(4569.974609, 56.528931, 12062.350586));
		RedGolems.Add(Vector3(4564.492188, 56.476685, 12284.696289));
		RedGolems.Add(Vector3(4746.958008, 56.476685, 12273.149414));
		RedGolems.Add(Vector3(5000.104492, 56.476807, 12113.990234));
		RedGolems.Add(Vector3(5617.074219, 56.469360, 12124.594727));
		RedGolems.Add(Vector3(6034.719727, 56.651978, 12297.876953));
		RedGolems.Add(Vector3(6204.883301, 56.621094, 12282.959961));
		RedGolems.Add(Vector3(6172.495605, 56.476807, 11999.903320));
		RedGolems.Add(Vector3(6607.081055, 56.476563, 12019.851563));
		RedGolems.Add(Vector3(6757.041504, 56.476685, 12437.867188));
		RedGolems.Add(Vector3(6971.717285, 56.476929, 12510.786133));
		RedGolems.Add(Vector3(7167.066406, 55.087646, 12471.289063));
		RedGolems.Add(Vector3(7238.115723, 56.476807, 12036.547852));

		WorldMapPolygons.emplace_back(RedGolems);

		Geometry::Polygon RedGolemsSingle;
		RedGolemsSingle.Add(Vector3(6014.227539, 52.842041, 12729.001953));
		RedGolemsSingle.Add(Vector3(5753.347656, 52.838135, 13128.929688));
		RedGolemsSingle.Add(Vector3(6625.967285, 52.842163, 13202.267578));
		RedGolemsSingle.Add(Vector3(6597.679688, 56.145386, 12848.964844));
		WorldMapPolygons.emplace_back(RedGolemsSingle);

		Geometry::Polygon RedTopT1;
		RedTopT1.Add(Vector3(4150.786133, 52.838257, 13114.156250));
		RedTopT1.Add(Vector3(5319.443848, 52.838257, 13095.816406));
		RedTopT1.Add(Vector3(5383.583008, 53.001343, 12509.322266));
		RedTopT1.Add(Vector3(5059.369141, 56.116089, 12529.881836));
		RedTopT1.Add(Vector3(4895.997559, 56.476685, 12670.422852));
		RedTopT1.Add(Vector3(4267.896484, 56.476685, 12728.522461));
		RedTopT1.Add(Vector3(4163.759277, 56.002930, 12826.313477));
		RedTopT1.Add(Vector3(4139.494629, 52.891479, 13022.264648));
		WorldMapPolygons.emplace_back(RedTopT1);

		Geometry::Polygon RedTopBig;
		RedTopBig.Add(Vector3(3783.379395, -45.627686, 11555.259766));
		RedTopBig.Add(Vector3(3200.939941, -45.295044, 11918.227539));
		RedTopBig.Add(Vector3(3016.609863, 52.838257, 12606.320313));
		RedTopBig.Add(Vector3(3595.523682, 53.018799, 12854.554688));
		RedTopBig.Add(Vector3(3872.610840, 56.483154, 12390.551758));
		RedTopBig.Add(Vector3(4176.215332, 49.194214, 11948.644531));
		RedTopBig.Add(Vector3(4139.115723, 44.317505, 11723.842773));
		RedTopBig.Add(Vector3(3938.129639, -9.465088, 11592.882813));

		WorldMapPolygons.emplace_back(RedTopBig);

		Geometry::Polygon Baron;

		Baron.Add(Vector3(4414.063477, -71.234375, 10159.683594));
		Baron.Add(Vector3(3865.174805, -79.326050, 10603.451172));
		Baron.Add(Vector3(3830.438965, -71.240845, 11119.303711));
		Baron.Add(Vector3(4384.916504, 49.172241, 11256.985352));
		Baron.Add(Vector3(4581.595703, 56.591309, 11595.612305));
		Baron.Add(Vector3(5338.002441, 56.840332, 11302.958984));
		Baron.Add(Vector3(5673.816406, 56.831909, 10983.867188));
		Baron.Add(Vector3(5752.845703, 55.703979, 10651.133789));
		Baron.Add(Vector3(5800.141602, 53.607666, 10336.331055));
		Baron.Add(Vector3(5791.242676, 19.586792, 10039.666016));
		Baron.Add(Vector3(6155.236816, 39.530151, 9658.230469));
		Baron.Add(Vector3(6038.671387, -52.815063, 9434.100586));
		Baron.Add(Vector3(5432.970215, -71.155273, 9543.729492));
		Baron.Add(Vector3(4854.492676, -71.072754, 9841.253906));
		Baron.Add(Vector3(4916.525391, -71.240723, 9987.624023));
		Baron.Add(Vector3(5221.647949, -71.240845, 9998.208008));
		Baron.Add(Vector3(5476.894043, -72.390747, 10405.250977));
		Baron.Add(Vector3(5359.370117, -71.168457, 10788.728516));
		Baron.Add(Vector3(4983.105957, -79.922852, 10966.291992));
		Baron.Add(Vector3(4583.304688, -71.240479, 10820.639648));
		Baron.Add(Vector3(4425.746094, -71.240723, 10459.007813));
		Baron.Add(Vector3(4539.324707, -71.240601, 10256.835938));
		WorldMapPolygons.emplace_back(Baron);

		Geometry::Polygon RedTopWall1;
		RedTopWall1.Add(Vector3(10414.212891, 91.429688, 14326.570313));
		RedTopWall1.Add(Vector3(10387.145508, 93.336914, 14660.301758));
		RedTopWall1.Add(Vector3(12232.330078, 93.336670, 14727.333984));
		RedTopWall1.Add(Vector3(12338.377930, 91.429932, 14363.128906));

		WorldMapPolygons.emplace_back(RedTopWall1);

		Geometry::Polygon RedTopWall2;
		RedTopWall2.Add(Vector3(10321.671875, 94.833008, 14185.653320));
		RedTopWall2.Add(Vector3(7656.854492, 52.837769, 14200.776367));
		RedTopWall2.Add(Vector3(7496.047852, 52.838013, 14689.400391));
		RedTopWall2.Add(Vector3(10526.651367, 93.336670, 14665.565430));

		WorldMapPolygons.emplace_back(RedTopWall2);

		Geometry::Polygon RedTopWall3;
		RedTopWall3.Add(Vector3(7694.961426, 52.838257, 14189.291992));
		RedTopWall3.Add(Vector3(5445.112793, 52.838135, 14164.584961));
		RedTopWall3.Add(Vector3(5332.544922, 52.838501, 14721.927734));
		RedTopWall3.Add(Vector3(7785.190430, 52.838379, 14747.904297));

		WorldMapPolygons.emplace_back(RedTopWall3);

		Geometry::Polygon RedTopWall4;
		RedTopWall4.Add(Vector3(5466.961914, 52.838135, 14145.047852));
		RedTopWall4.Add(Vector3(3494.477051, 52.838623, 14087.623047));
		RedTopWall4.Add(Vector3(3462.488525, 52.838379, 14596.532227));
		RedTopWall4.Add(Vector3(5551.662109, 52.838257, 14683.535156));

		WorldMapPolygons.emplace_back(RedTopWall4);

		Geometry::Polygon RedTopWall5;
		RedTopWall5.Add(Vector3(3980.598389, 52.838013, 14132.327148));
		RedTopWall5.Add(Vector3(2105.365723, 55.855835, 13603.581055));
		RedTopWall5.Add(Vector3(1757.416748, 58.144897, 13852.615234));
		RedTopWall5.Add(Vector3(3903.902588, 52.838379, 14662.561523));

		WorldMapPolygons.emplace_back(RedTopWall5);

		Geometry::Polygon TopAlcove;
		TopAlcove.Add(Vector3(1892.253906, 57.792969, 13756.916992));
		TopAlcove.Add(Vector3(1446.341797, 57.960815, 13992.013672));
		TopAlcove.Add(Vector3(728.685181, 55.646240, 13396.438477));
		TopAlcove.Add(Vector3(778.683472, 55.692505, 13042.398438));
		TopAlcove.Add(Vector3(977.787476, 52.837891, 12671.968750));
		TopAlcove.Add(Vector3(982.401367, 52.838013, 12371.001953));
		TopAlcove.Add(Vector3(558.070190, 52.838013, 12514.747070));
		TopAlcove.Add(Vector3(294.635498, 52.838135, 13348.700195));
		TopAlcove.Add(Vector3(1247.163940, -45.387085, 14567.636719));
		TopAlcove.Add(Vector3(2367.402832, 54.033813, 13768.486328));
		WorldMapPolygons.emplace_back(TopAlcove);

		Geometry::Polygon TopAlcoveSingle;
		TopAlcoveSingle.Add(Vector3(1766.855713, 48.514648, 13226.632813));
		TopAlcoveSingle.Add(Vector3(1429.713501, 52.838379, 12910.822266));
		TopAlcoveSingle.Add(Vector3(1153.166870, 57.660278, 13161.521484));
		TopAlcoveSingle.Add(Vector3(1511.571167, 58.543213, 13528.681641));
		WorldMapPolygons.emplace_back(TopAlcoveSingle);

		//blue top

		Geometry::Polygon BlueTopWall1;
		BlueTopWall1.Add(Vector3(127.562469, 93.375732, 2265.419922));
		BlueTopWall1.Add(Vector3(72.850739, 93.375610, 4051.663574));
		BlueTopWall1.Add(Vector3(500.432312, 95.748047, 3883.495850));
		BlueTopWall1.Add(Vector3(492.559326, 95.748169, 2199.891357));
		WorldMapPolygons.emplace_back(BlueTopWall1);

		Geometry::Polygon BlueTopWall2;
		BlueTopWall2.Add(Vector3(490.917297, 95.748047, 3673.657959));
		BlueTopWall2.Add(Vector3(52.156555, 93.375610, 3724.229004));
		BlueTopWall2.Add(Vector3(148.622314, 53.116333, 5728.599121));
		BlueTopWall2.Add(Vector3(734.217163, 53.143555, 5721.658691));
		WorldMapPolygons.emplace_back(BlueTopWall2);

		Geometry::Polygon BlueTopWall3;
		BlueTopWall3.Add(Vector3(727.152466, 53.154907, 5697.246094));
		BlueTopWall3.Add(Vector3(259.223999, 53.080322, 5813.752441));
		BlueTopWall3.Add(Vector3(148.377747, 52.838013, 7443.738281));
		BlueTopWall3.Add(Vector3(698.184570, 52.838623, 7438.337402));
		WorldMapPolygons.emplace_back(BlueTopWall3);

		Geometry::Polygon BlueTopWall4;
		BlueTopWall4.Add(Vector3(689.951233, 52.838257, 7384.907227));
		BlueTopWall4.Add(Vector3(270.235840, 52.838379, 7458.145508));
		BlueTopWall4.Add(Vector3(262.871765, 52.837891, 9033.538086));
		BlueTopWall4.Add(Vector3(688.577209, 52.838257, 8964.795898));
		WorldMapPolygons.emplace_back(BlueTopWall4);

		Geometry::Polygon BlueTopWall5;
		BlueTopWall5.Add(Vector3(661.258179, 52.838013, 8967.731445));
		BlueTopWall5.Add(Vector3(293.353210, 52.838135, 9016.861328));
		BlueTopWall5.Add(Vector3(248.946960, 52.838257, 10608.316406));
		BlueTopWall5.Add(Vector3(717.284180, 52.838135, 10551.039063));
		WorldMapPolygons.emplace_back(BlueTopWall5);

		Geometry::Polygon BlueTopWall6;
		BlueTopWall6.Add(Vector3(706.099854, 52.838135, 10409.108398));
		BlueTopWall6.Add(Vector3(346.209778, 52.838379, 10497.449219));
		BlueTopWall6.Add(Vector3(521.207275, 52.838135, 12764.983398));
		BlueTopWall6.Add(Vector3(971.023560, 52.838257, 12510.726563));
		WorldMapPolygons.emplace_back(BlueTopWall6);

		Geometry::Polygon BlueMidT1;
		BlueMidT1.Add(Vector3(5205.732422, 50.460693, 6172.945801));
		BlueMidT1.Add(Vector3(4805.275391, 50.673828, 6243.805176));
		BlueMidT1.Add(Vector3(4983.647949, 50.566528, 6707.740234));
		BlueMidT1.Add(Vector3(4828.701660, 50.541382, 7360.472168));
		BlueMidT1.Add(Vector3(5142.764160, 50.734009, 7567.479492));
		BlueMidT1.Add(Vector3(5652.708984, 51.654785, 7324.076172));
		BlueMidT1.Add(Vector3(5945.368164, 51.802734, 6877.393066));
		BlueMidT1.Add(Vector3(5225.213379, 50.408691, 6182.913086));

		WorldMapPolygons.emplace_back(BlueMidT1);

		Geometry::Polygon BlueBlueBuff;
		BlueBlueBuff.Add(Vector3(3191.705078, 51.880737, 7488.894531));
		BlueBlueBuff.Add(Vector3(3220.987061, 51.906128, 7682.764160));
		BlueBlueBuff.Add(Vector3(4037.505859, 51.779419, 7686.029297));
		BlueBlueBuff.Add(Vector3(3999.743408, 50.822754, 8071.231934));
		BlueBlueBuff.Add(Vector3(3827.487305, 50.859985, 8126.393066));
		BlueBlueBuff.Add(Vector3(3803.894775, 65.710571, 8692.666992));
		BlueBlueBuff.Add(Vector3(3685.966797, 34.253906, 9057.400391));
		BlueBlueBuff.Add(Vector3(3914.905273, -66.604858, 9421.014648));
		BlueBlueBuff.Add(Vector3(4779.886230, -68.706909, 8843.420898));
		BlueBlueBuff.Add(Vector3(4335.776855, 48.690674, 8370.791016));
		BlueBlueBuff.Add(Vector3(4569.584961, 52.310425, 7697.086914));
		BlueBlueBuff.Add(Vector3(4355.655762, 50.299438, 7470.887695));
		BlueBlueBuff.Add(Vector3(3240.766357, 51.888062, 7481.326660));

		WorldMapPolygons.emplace_back(BlueBlueBuff);

		Geometry::Polygon BlueWolves;
		BlueWolves.Add(Vector3(4132.314453, 52.500000, 6113.786133));
		BlueWolves.Add(Vector3(3942.330322, 52.466187, 6257.706055));
		BlueWolves.Add(Vector3(4109.680664, 52.467163, 6478.029785));
		BlueWolves.Add(Vector3(3855.948730, 52.459961, 6729.646973));
		BlueWolves.Add(Vector3(3474.754883, 51.910522, 6735.188477));
		BlueWolves.Add(Vector3(3427.330566, 51.270264, 6843.044922));
		BlueWolves.Add(Vector3(3427.330566, 51.270264, 6843.044922));
		BlueWolves.Add(Vector3(4257.799805, 50.649048, 7015.747559));
		BlueWolves.Add(Vector3(4567.033691, 51.497437, 6802.081055));
		BlueWolves.Add(Vector3(4472.931152, 50.734131, 6383.468750));
		BlueWolves.Add(Vector3(4092.230713, 52.496094, 6107.388184));

		WorldMapPolygons.emplace_back(BlueWolves);

		Geometry::Polygon BlueTopFirstWall;
		BlueTopFirstWall.Add(Vector3(3026.675537, 51.883789, 7036.697266));
		BlueTopFirstWall.Add(Vector3(3067.160400, 51.283569, 6485.329590));
		BlueTopFirstWall.Add(Vector3(3448.942627, 52.485840, 5980.851563));
		BlueTopFirstWall.Add(Vector3(3960.176025, 52.503540, 5762.472656));
		BlueTopFirstWall.Add(Vector3(4408.088867, 52.496582, 5911.927734));
		BlueTopFirstWall.Add(Vector3(4843.512207, 50.341553, 5800.015137));
		BlueTopFirstWall.Add(Vector3(4860.837891, 50.233276, 5747.239258));
		BlueTopFirstWall.Add(Vector3(4168.205566, 50.243896, 5069.189941));
		BlueTopFirstWall.Add(Vector3(2977.917480, 52.649048, 5415.462891));
		BlueTopFirstWall.Add(Vector3(3135.617920, 57.045166, 5850.151367));
		BlueTopFirstWall.Add(Vector3(3038.728760, 57.046753, 6192.059082));
		BlueTopFirstWall.Add(Vector3(2819.026611, 57.039429, 6342.925781));
		BlueTopFirstWall.Add(Vector3(2836.845215, 50.622192, 7081.673828));
		WorldMapPolygons.emplace_back(BlueTopFirstWall);

		Geometry::Polygon BlueTopBase;
		BlueTopBase.Add(Vector3(1673.659058, 95.766968, 4783.750977));
		BlueTopBase.Add(Vector3(1685.485962, 63.298706, 5056.498047));
		BlueTopBase.Add(Vector3(2573.176514, 53.102295, 4981.761230));
		BlueTopBase.Add(Vector3(2991.368164, 53.546875, 4971.118652));
		BlueTopBase.Add(Vector3(3773.986816, 105.451172, 4493.261230));
		BlueTopBase.Add(Vector3(3632.378662, 95.748169, 4287.553223));
		BlueTopBase.Add(Vector3(2897.743408, 93.259766, 4672.315430));
		BlueTopBase.Add(Vector3(2475.296875, 91.600220, 4724.768555));
		BlueTopBase.Add(Vector3(1727.588623, 95.074219, 4768.143066));
		WorldMapPolygons.emplace_back(BlueTopBase);

		Geometry::Polygon BlueTopT2;
		BlueTopT2.Add(Vector3(1751.265259, 56.151733, 5587.091797));
		BlueTopT2.Add(Vector3(1813.598389, 52.360352, 7536.434082));
		BlueTopT2.Add(Vector3(2452.666992, 49.843750, 7151.437500));
		BlueTopT2.Add(Vector3(2482.606689, 57.035889, 5937.386719));
		BlueTopT2.Add(Vector3(2630.661133, 55.731079, 5845.692871));
		BlueTopT2.Add(Vector3(2543.900635, 52.761475, 5585.449219));
		BlueTopT2.Add(Vector3(2392.500244, 53.147949, 5524.027832));
		BlueTopT2.Add(Vector3(2392.500244, 53.147949, 5524.027832));

		WorldMapPolygons.emplace_back(BlueTopT2);

		Geometry::Polygon BlueGromp;
		BlueGromp.Add(Vector3(2564.398682, 50.447266, 7595.792480));
		BlueGromp.Add(Vector3(1721.588501, 52.838135, 8127.506348));
		BlueGromp.Add(Vector3(1752.133301, 52.838135, 9354.541016));
		BlueGromp.Add(Vector3(2221.223877, 51.775269, 9412.461914));
		BlueGromp.Add(Vector3(1981.616943, 51.777344, 8638.029297));
		BlueGromp.Add(Vector3(2023.926025, 51.776855, 8300.125977));
		BlueGromp.Add(Vector3(2717.099121, 51.859741, 7963.351074));

		WorldMapPolygons.emplace_back(BlueGromp);

		Geometry::Polygon BlueFrontBlueBuff;
		BlueFrontBlueBuff.Add(Vector3(2507.343262, 51.777466, 8641.099609));
		BlueFrontBlueBuff.Add(Vector3(2608.955811, 54.325195, 9872.094727));
		BlueFrontBlueBuff.Add(Vector3(2708.653320, 54.022949, 9857.390625));
		BlueFrontBlueBuff.Add(Vector3(2866.483643, 50.677002, 9058.101563));
		BlueFrontBlueBuff.Add(Vector3(3351.771973, 51.038940, 8620.697266));
		BlueFrontBlueBuff.Add(Vector3(3307.156006, 51.674438, 8478.575195));
		BlueFrontBlueBuff.Add(Vector3(3065.751465, 51.767822, 8346.722656));
		BlueFrontBlueBuff.Add(Vector3(2614.711914, 51.773560, 8538.927734));

		WorldMapPolygons.emplace_back(BlueFrontBlueBuff);

		Geometry::Polygon BlueTopBig;
		BlueTopBig.Add(Vector3(1829.137817, 52.838257, 9839.287109));
		BlueTopBig.Add(Vector3(1853.636108, 52.838379, 11168.759766));
		BlueTopBig.Add(Vector3(2061.936035, 52.837891, 11577.183594));
		BlueTopBig.Add(Vector3(2335.820557, 28.824341, 11695.448242));
		BlueTopBig.Add(Vector3(2599.790527, -27.947266, 11488.557617));
		BlueTopBig.Add(Vector3(2957.260010, -73.047485, 10859.950195));
		BlueTopBig.Add(Vector3(3400.537842, -57.919312, 9882.091797));
		BlueTopBig.Add(Vector3(3467.274658, 8.268433, 9438.175781));
		BlueTopBig.Add(Vector3(3313.142578, 46.455933, 9259.928711));
		BlueTopBig.Add(Vector3(3216.762939, 49.669189, 9303.915039));
		BlueTopBig.Add(Vector3(3273.915283, 18.939331, 9656.650391));
		BlueTopBig.Add(Vector3(2927.075195, 35.955322, 10264.078125));
		BlueTopBig.Add(Vector3(2736.371582, 54.300049, 10408.945313));
		BlueTopBig.Add(Vector3(2423.086670, 54.325562, 10415.620117));
		BlueTopBig.Add(Vector3(2294.309814, 54.325684, 10191.588867));
		BlueTopBig.Add(Vector3(2113.694336, 52.794312, 9935.145508));
		BlueTopBig.Add(Vector3(2030.814331, 52.838135, 9851.607422));
		BlueTopBig.Add(Vector3(1905.932983, 52.838135, 9831.685547));

		WorldMapPolygons.emplace_back(BlueTopBig);

		WorldMap = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(WorldMapPolygons));
	}

	std::vector<DetectedSpell>DetectedSpells;
	std::vector<DetectedSpell>DodgeableSpells;
	float NewTimer = 0;
	float OldTimer = 0;
	Vector3 SafePos;
	Vector3 ExtendedPos;
	Vector3 MyHeroPos;
	bool Evading = false;
	float Bounding = 65;
	std::vector<Vector3>SafePositions;
	float GameTimer = 0;

	void AddSpell(Geometry::Polygon& Path,
		Geometry::Polygon& Path2,
		Vector3& StartPos,
		Vector3& EndPos,
		Spell& Spell
	)
	{
		float sT = GameTimer - M.Evade.GP / 1000;
		DetectedSpell spell(Path,
			Path2,
			StartPos,
			EndPos,
			Spell,
			sT
		);
		DetectedSpells.emplace_back(spell);
	}

	void InitEvadeSpells()
	{
		{
			EvadeSpellDB Ahri;
			Ahri.hero = "Ahri";

			EvadeSpell R;
			R.icon = "AhriR";
			R.type = 1;
			R.displayName = "Spirit Rush";
			R.name = "AhriR";
			R.danger = 4;
			R.range = 450;
			R.slot = _R;

			Ahri.spells.emplace_back(R);

			evadespelldb.emplace_back(Ahri);
		}

		{
			EvadeSpellDB Annie;
			Annie.hero = "Annie";

			EvadeSpell E;
			E.icon = "AnnieE";
			E.type = 2;
			E.displayName = "Molten Shield";
			E.name = "AnnieE";
			E.danger = 2;
			E.range = 0;
			E.slot = _E;

			Annie.spells.emplace_back(E);

			evadespelldb.emplace_back(Annie);
		}

		{
			EvadeSpellDB Blitzcrank;
			Blitzcrank.hero = "Blitzcrank";

			EvadeSpell W;
			W.icon = "BlitzcrankW";
			W.type = 2;
			W.displayName = "Overdrive";
			W.name = "BlitzcrankW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Blitzcrank.spells.emplace_back(W);

			evadespelldb.emplace_back(Blitzcrank);
		}

		{
			EvadeSpellDB Corki;
			Corki.hero = "Corki";

			EvadeSpell W;
			W.icon = "CorkiW";
			W.type = 1;
			W.displayName = "Valkyrie";
			W.name = "CorkiW";
			W.danger = 4;
			W.range = 600;
			W.slot = _W;

			Corki.spells.emplace_back(W);

			evadespelldb.emplace_back(Corki);
		}

		{
			EvadeSpellDB Draven;
			Draven.hero = "Draven";

			EvadeSpell W;
			W.icon = "DravenW";
			W.type = 2;
			W.displayName = "Blood Rush";
			W.name = "DravenW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Draven.spells.emplace_back(W);

			evadespelldb.emplace_back(Draven);
		}

		{
			EvadeSpellDB Ekko;
			Ekko.hero = "Ekko";

			EvadeSpell E;
			E.icon = "EkkoE";
			E.type = 1;
			E.displayName = "Phase Dive";
			E.name = "EkkoE";
			E.danger = 2;
			E.range = 325;
			E.slot = _E;

			Ekko.spells.emplace_back(E);

			evadespelldb.emplace_back(Ekko);
		}

		{
			EvadeSpellDB Ezreal;
			Ezreal.hero = "Ezreal";

			EvadeSpell E;
			E.icon = "EzrealE";
			E.type = 1;
			E.displayName = "Arcane Shift";
			E.name = "EzrealE";
			E.danger = 3;
			E.range = 475;
			E.slot = _E;

			Ezreal.spells.emplace_back(E);

			evadespelldb.emplace_back(Ezreal);
		}

		{
			EvadeSpellDB Fiora;
			Fiora.hero = "Fiora";

			EvadeSpell Q;
			Q.icon = "FioraQ";
			Q.type = 1;
			Q.displayName = "Lunge";
			Q.name = "FioraQ";
			Q.danger = 1;
			Q.range = 400;
			Q.slot = _Q;

			Fiora.spells.emplace_back(Q);

			EvadeSpell W;
			W.icon = "FioraW";
			W.type = 7;
			W.displayName = "Riposte";
			W.name = "FioraW";
			W.danger = 2;
			W.range = 750;
			W.slot = _W;

			Fiora.spells.emplace_back(W);

			evadespelldb.emplace_back(Fiora);
		}

		{
			EvadeSpellDB Fizz;
			Fizz.hero = "Fizz";

			EvadeSpell E;
			E.icon = "FizzE";
			E.type = 3;
			E.displayName = "Playful";
			E.name = "FizzE";
			E.danger = 3;
			E.range = 0;
			E.slot = _E;

			Fizz.spells.emplace_back(E);

			evadespelldb.emplace_back(Fizz);
		}

		{
			EvadeSpellDB Garen;
			Garen.hero = "Garen";

			EvadeSpell Q;
			Q.icon = "GarenQ";
			Q.type = 2;
			Q.displayName = "Decisive Strike";
			Q.name = "GarenQ";
			Q.danger = 3;
			Q.range = 0;
			Q.slot = _Q;

			Garen.spells.emplace_back(Q);

			evadespelldb.emplace_back(Garen);
		}

		{
			EvadeSpellDB Gnar;
			Gnar.hero = "Gnar";

			EvadeSpell E;
			E.icon = "GnarE";
			E.type = 1;
			E.displayName = "Hop/Crunch";
			E.name = "GnarE";
			E.danger = 3;
			E.range = 475;
			E.slot = _E;

			Gnar.spells.emplace_back(E);

			evadespelldb.emplace_back(Gnar);
		}

		{
			EvadeSpellDB Gragas;
			Gragas.hero = "Gragas";

			EvadeSpell E;
			E.icon = "GragasE";
			E.type = 1;
			E.displayName = "Body Slam";
			E.name = "GragasE";
			E.danger = 3;
			E.range = 600;
			E.slot = _E;

			Gragas.spells.emplace_back(E);

			evadespelldb.emplace_back(Gragas);
		}

		{
			EvadeSpellDB Graves;
			Graves.hero = "Graves";

			EvadeSpell E;
			E.icon = "GravesE";
			E.type = 1;
			E.displayName = "Quickdraw";
			E.name = "GravesE";
			E.danger = 1;
			E.range = 425;
			E.slot = _E;

			Graves.spells.emplace_back(E);

			evadespelldb.emplace_back(Graves);
		}

		{
			EvadeSpellDB Kaisa;
			Kaisa.hero = "Kaisa";

			EvadeSpell E;
			E.icon = "KaisaE";
			E.type = 2;
			E.displayName = "Supercharge";
			E.name = "KaisaE";
			E.danger = 2;
			E.range = 0;
			E.slot = _E;

			Kaisa.spells.emplace_back(E);

			evadespelldb.emplace_back(Kaisa);
		}

		{
			EvadeSpellDB Karma;
			Karma.hero = "Karma";

			EvadeSpell E;
			E.icon = "KarmaE";
			E.type = 2;
			E.displayName = "Inspire";
			E.name = "KarmaE";
			E.danger = 3;
			E.range = 0;
			E.slot = _E;

			Karma.spells.emplace_back(E);

			evadespelldb.emplace_back(Karma);
		}

		{
			EvadeSpellDB Kassadin;
			Kassadin.hero = "Kassadin";

			EvadeSpell R;
			R.icon = "KassadinR";
			R.type = 1;
			R.displayName = "Riftwalk";
			R.name = "KassadinR";
			R.danger = 3;
			R.range = 500;
			R.slot = _R;

			Kassadin.spells.emplace_back(R);

			evadespelldb.emplace_back(Kassadin);
		}

		{
			EvadeSpellDB Katarina;
			Katarina.hero = "Katarina";

			EvadeSpell W;
			W.icon = "KatarinaW";
			W.type = 2;
			W.displayName = "Preparation";
			W.name = "KatarinaW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Katarina.spells.emplace_back(W);

			evadespelldb.emplace_back(Katarina);
		}

		{
			EvadeSpellDB Kayn;
			Kayn.hero = "Kayn";

			EvadeSpell Q;
			Q.icon = "KaynQ";
			Q.type = 1;
			Q.displayName = "Reaping Slash";
			Q.name = "KaynQ";
			Q.danger = 2;
			Q.range = 350;
			Q.slot = _Q;

			Kayn.spells.emplace_back(Q);

			evadespelldb.emplace_back(Kayn);
		}

		{
			EvadeSpellDB Kennen;
			Kennen.hero = "Kennen";

			EvadeSpell E;
			E.icon = "KennenE";
			E.type = 2;
			E.displayName = "Lightning Rush";
			E.name = "KennenE";
			E.danger = 3;
			E.range = 0;
			E.slot = _E;

			Kennen.spells.emplace_back(E);

			evadespelldb.emplace_back(Kennen);
		}

		{
			EvadeSpellDB Khazix;
			Khazix.hero = "Khazix";

			EvadeSpell E;
			E.icon = "KhazixE";
			E.type = 1;
			E.displayName = "Leap";
			E.name = "KhazixE";
			E.danger = 3;
			E.range = 700;
			E.slot = _E;

			Khazix.spells.emplace_back(E);

			evadespelldb.emplace_back(Khazix);
		}

		{
			EvadeSpellDB Kindred;
			Kindred.hero = "Kindred";

			EvadeSpell Q;
			Q.icon = "KindredQ";
			Q.type = 1;
			Q.displayName = "Dance of Arrows";
			Q.name = "KindredQ";
			Q.danger = 1;
			Q.range = 340;
			Q.slot = _Q;

			Kindred.spells.emplace_back(Q);

			evadespelldb.emplace_back(Kindred);
		}

		{
			EvadeSpellDB Kled;
			Kled.hero = "Kled";

			EvadeSpell E;
			E.icon = "KledE";
			E.type = 1;
			E.displayName = "Jousting";
			E.name = "KledE";
			E.danger = 3;
			E.range = 550;
			E.slot = _E;

			Kled.spells.emplace_back(E);

			evadespelldb.emplace_back(Kled);
		}

		{
			EvadeSpellDB Leblanc;
			Leblanc.hero = "Leblanc";

			EvadeSpell W;
			W.icon = "LeblancW";
			W.type = 1;
			W.displayName = "Distortion";
			W.name = "LeblancW";
			W.danger = 3;
			W.range = 600;
			W.slot = _W;

			Leblanc.spells.emplace_back(W);

			evadespelldb.emplace_back(Leblanc);
		}

		{
			EvadeSpellDB Lucian;
			Lucian.hero = "Lucian";

			EvadeSpell E;
			E.icon = "LucianE";
			E.type = 1;
			E.displayName = "Relentless Pursuit";
			E.name = "LucianE";
			E.danger = 3;
			E.range = 425;
			E.slot = _E;

			Lucian.spells.emplace_back(E);

			evadespelldb.emplace_back(Lucian);
		}

		{
			EvadeSpellDB MasterYi;
			MasterYi.hero = "MasterYi";

			EvadeSpell Q;
			Q.icon = "MasterYiQ";
			Q.type = 4;
			Q.displayName = "Alpha Strike";
			Q.name = "MasterYiQ";
			Q.danger = 3;
			Q.range = 600;
			Q.slot = _Q;

			MasterYi.spells.emplace_back(Q);

			evadespelldb.emplace_back(MasterYi);
		}

		{
			EvadeSpellDB Morgana;
			Morgana.hero = "Morgana";

			EvadeSpell E;
			E.icon = "MorganaE";
			E.type = 5;
			E.displayName = "Black Shield";
			E.name = "MorganaE";
			E.danger = 2;
			E.range = 0;
			E.slot = _E;

			Morgana.spells.emplace_back(E);

			evadespelldb.emplace_back(Morgana);
		}

		{
			EvadeSpellDB Pyke;
			Pyke.hero = "Pyke";

			EvadeSpell E;
			E.icon = "PykeE";
			E.type = 1;
			E.displayName = "Phantom Undertow";
			E.name = "PykeE";
			E.danger = 3;
			E.range = 550;
			E.slot = _E;

			Pyke.spells.emplace_back(E);

			evadespelldb.emplace_back(Pyke);
		}

		//todo E spell type 8 using to allies in range
		{
			EvadeSpellDB Rakan;
			Rakan.hero = "Rakan";

			EvadeSpell W;
			W.icon = "RakanW";
			W.type = 1;
			W.displayName = "Grand Entrance";
			W.name = "RakanW";
			W.danger = 3;
			W.range = 600;
			W.slot = _W;

			Rakan.spells.emplace_back(W);

			evadespelldb.emplace_back(Rakan);
		}

		{
			EvadeSpellDB Renekton;
			Renekton.hero = "Renekton";

			EvadeSpell E;
			E.icon = "RenektonE";
			E.type = 1;
			E.displayName = "Slice and Dice";
			E.name = "RenektonE";
			E.danger = 3;
			E.range = 450;
			E.slot = _E;

			Renekton.spells.emplace_back(E);

			evadespelldb.emplace_back(Renekton);
		}

		{
			EvadeSpellDB Riven;
			Riven.hero = "Riven";

			EvadeSpell E;
			E.icon = "RivenE";
			E.type = 1;
			E.displayName = "Valor";
			E.name = "RivenE";
			E.danger = 2;
			E.range = 325;
			E.slot = _E;

			Riven.spells.emplace_back(E);

			evadespelldb.emplace_back(Riven);
		}

		{
			EvadeSpellDB Rumble;
			Rumble.hero = "Rumble";

			EvadeSpell W;
			W.icon = "RumbleW";
			W.type = 2;
			W.displayName = "Scrap Shield";
			W.name = "RumbleW";
			W.danger = 2;
			W.range = 0;
			W.slot = _W;

			Rumble.spells.emplace_back(W);

			evadespelldb.emplace_back(Rumble);
		}

		{
			EvadeSpellDB Sejuani;
			Sejuani.hero = "Sejuani";

			EvadeSpell Q;
			Q.icon = "SejuaniQ";
			Q.type = 1;
			Q.displayName = "Arctic Assault";
			Q.name = "SejuaniQ";
			Q.danger = 3;
			Q.range = 650;
			Q.slot = _Q;

			Sejuani.spells.emplace_back(Q);

			evadespelldb.emplace_back(Sejuani);
		}

		{
			EvadeSpellDB Shaco;
			Shaco.hero = "Shaco";

			EvadeSpell Q;
			Q.icon = "ShacoQ";
			Q.type = 1;
			Q.displayName = "Deceive";
			Q.name = "ShacoQ";
			Q.danger = 3;
			Q.range = 400;
			Q.slot = _Q;

			Shaco.spells.emplace_back(Q);

			evadespelldb.emplace_back(Shaco);
		}

		{
			EvadeSpellDB Shen;
			Shen.hero = "Shen";

			EvadeSpell E;
			E.icon = "ShenE";
			E.type = 1;
			E.displayName = "Shadow Dash";
			E.name = "ShenE";
			E.danger = 4;
			E.range = 600;
			E.slot = _E;

			Shen.spells.emplace_back(E);

			evadespelldb.emplace_back(Shen);
		}

		{
			EvadeSpellDB Shyvana;
			Shyvana.hero = "Shyvana";

			EvadeSpell W;
			W.icon = "ShyvanaW";
			W.type = 2;
			W.displayName = "Burnout";
			W.name = "ShyvanaW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Shyvana.spells.emplace_back(W);

			evadespelldb.emplace_back(Shyvana);
		}

		{
			EvadeSpellDB Sivir;
			Sivir.hero = "Sivir";

			EvadeSpell E;
			E.icon = "SivirE";
			E.type = 5;
			E.displayName = "Spell Shield";
			E.name = "SivirE";
			E.danger = 2;
			E.range = 0;
			E.slot = _E;

			Sivir.spells.emplace_back(E);

			evadespelldb.emplace_back(Sivir);
		}

		{
			EvadeSpellDB Skarner;
			Skarner.hero = "Skarner";

			EvadeSpell W;
			W.icon = "SkarnerW";
			W.type = 2;
			W.displayName = "Crystalline Exoskeleton";
			W.name = "SkarnerW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Skarner.spells.emplace_back(W);

			evadespelldb.emplace_back(Skarner);
		}

		{
			EvadeSpellDB Sona;
			Sona.hero = "Sona";

			EvadeSpell E;
			E.icon = "SonaE";
			E.type = 2;
			E.displayName = "Song of Celerity";
			E.name = "SonaE";
			E.danger = 3;
			E.range = 0;
			E.slot = _E;

			Sona.spells.emplace_back(E);

			evadespelldb.emplace_back(Sona);
		}

		{
			EvadeSpellDB Teemo;
			Teemo.hero = "Teemo";

			EvadeSpell W;
			W.icon = "TeemoW";
			W.type = 2;
			W.displayName = "Move Quick";
			W.name = "TeemoW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Teemo.spells.emplace_back(W);

			evadespelldb.emplace_back(Teemo);
		}

		{
			EvadeSpellDB Tryndamere;
			Tryndamere.hero = "Tryndamere";

			EvadeSpell E;
			E.icon = "TryndamereE";
			E.type = 1;
			E.displayName = "Spinning Slash";
			E.name = "TryndamereE";
			E.danger = 3;
			E.range = 660;
			E.slot = _E;

			Tryndamere.spells.emplace_back(E);

			evadespelldb.emplace_back(Tryndamere);
		}

		{
			EvadeSpellDB Udyr;
			Udyr.hero = "Udyr";

			EvadeSpell E;
			E.icon = "UdyrE";
			E.type = 2;
			E.displayName = "Bear Stance";
			E.name = "UdyrE";
			E.danger = 1;
			E.range = 0;
			E.slot = _E;

			Udyr.spells.emplace_back(E);

			evadespelldb.emplace_back(Udyr);
		}

		{
			EvadeSpellDB Vayne;
			Vayne.hero = "Vayne";

			EvadeSpell Q;
			Q.icon = "VayneQ";
			Q.type = 1;
			Q.displayName = "Tumble";
			Q.name = "VayneQ";
			Q.danger = 2;
			Q.range = 300;
			Q.slot = _Q;

			Vayne.spells.emplace_back(Q);

			evadespelldb.emplace_back(Vayne);
		}

		{
			EvadeSpellDB Vi;
			Vi.hero = "Vi";

			EvadeSpell Q;
			Q.icon = "ViQ";
			Q.type = 1;
			Q.displayName = "Vault Breaker";
			Q.name = "ViQ";
			Q.danger = 3;
			Q.range = 250;
			Q.slot = _Q;

			Vi.spells.emplace_back(Q);

			evadespelldb.emplace_back(Vi);
		}

		{
			EvadeSpellDB Vladimir;
			Vladimir.hero = "Vladimir";

			EvadeSpell W;
			W.icon = "VladimirW";
			W.type = 3;
			W.displayName = "Sanguine Pool";
			W.name = "VladimirW";
			W.danger = 3;
			W.range = 0;
			W.slot = _W;

			Vladimir.spells.emplace_back(W);

			evadespelldb.emplace_back(Vladimir);
		}

		{
			EvadeSpellDB Volibear;
			Volibear.hero = "Volibear";

			EvadeSpell Q;
			Q.icon = "VolibearQ";
			Q.type = 2;
			Q.displayName = "Rolling Thunder";
			Q.name = "VolibearQ";
			Q.danger = 3;
			Q.range = 0;
			Q.slot = _Q;

			Volibear.spells.emplace_back(Q);

			evadespelldb.emplace_back(Volibear);
		}

		{
			EvadeSpellDB Xayah;
			Xayah.hero = "Xayah";

			EvadeSpell R;
			R.icon = "XayahR";
			R.type = 3;
			R.displayName = "Featherstorm";
			R.name = "XayahR";
			R.danger = 5;
			R.range = 0;
			R.slot = _R;

			Xayah.spells.emplace_back(R);

			evadespelldb.emplace_back(Xayah);
		}

		{
			EvadeSpellDB Yasuo;
			Yasuo.hero = "Yasuo";

			EvadeSpell W;
			W.icon = "YasuoW";
			W.type = 6;
			W.displayName = "Wind Wall";
			W.name = "YasuoW";
			W.danger = 2;
			W.range = 0;
			W.slot = _W;

			Yasuo.spells.emplace_back(W);

			evadespelldb.emplace_back(Yasuo);
		}

		//maybe add w todo
		{
			EvadeSpellDB Zed;
			Zed.hero = "Zed";

			EvadeSpell R;
			R.icon = "ZedR";
			R.type = 4;
			R.displayName = "Death Mark";
			R.name = "ZedR";
			R.danger = 4;
			R.range = 625;
			R.slot = _R;

			Zed.spells.emplace_back(R);

			evadespelldb.emplace_back(Zed);
		}

		{
			EvadeSpellDB Zilean;
			Zilean.hero = "Zilean";

			EvadeSpell E;
			E.icon = "ZileanE";
			E.type = 2;
			E.displayName = "Time Warp";
			E.name = "ZileanE";
			E.danger = 4;
			E.range = 0;
			E.slot = _E;

			Zilean.spells.emplace_back(E);

			evadespelldb.emplace_back(Zilean);
		}

		/*if (utils->ToLower(Local.SummonerSpell1()) == utils->ToLower("SummonerFlash"))
		{
			EvadeSpell Flash;
			Flash.name = "Flash";
			Flash.icon = "Flash";
			Flash.displayName = "Summoner Flash";
			Flash.slot = _D;
			Flash.range = 400;

			for (auto h : evadespelldb)
			{
				h.spells.emplace_back(Flash);
			}
		}
		else if (utils->ToLower(Local.SummonerSpell2()) == utils->ToLower("SummonerFlash"))
		{
			EvadeSpell Flash;
			Flash.name = "Flash";
			Flash.icon = "Flash";
			Flash.displayName = "Summoner Flash";
			Flash.slot = _F;
			Flash.range = 400;

			for (auto h : evadespelldb)
			{
				h.spells.emplace_back(Flash);
			}
		}*/
	}

	void InitSpells()
	{
		{
			Champ Aatrox;
			Aatrox.hero = "Aatrox";

			Spell Q1;
			Q1.name = "AatroxQ";
			Q1.icon = "AatroxQ1";
			Q1.displayName = "The Darkin Blade [First]";
			Q1.missileName = "";
			Q1.slot = _Q;
			Q1.type = linear;
			Q1.speed = MathHuge;
			Q1.range = 650;
			Q1.delay = 0.6;
			Q1.radius = 130;
			Q1.angle = 0;
			Q1.danger = 3;
			Q1.cc = true;
			Q1.collision = false;
			Q1.windwall = false;
			Q1.hitbox = false;
			Q1.fow = false;
			Q1.exception = false;
			Q1.extend = true;
			Aatrox.spells.emplace_back(Q1);

			Spell Q2;
			Q2.name = "AatroxQ2";
			Q2.icon = "AatroxQ2";
			Q2.displayName = "The Darkin Blade [Second]";
			Q2.missileName = "";
			Q2.slot = _Q;
			Q2.type = polygon;
			Q2.speed = MathHuge;
			Q2.range = 500;
			Q2.delay = 0.6;
			Q2.radius = 200;
			Q2.angle = 0;
			Q2.danger = 3;
			Q2.cc = true;
			Q2.collision = false;
			Q2.windwall = false;
			Q2.hitbox = false;
			Q2.fow = false;
			Q2.exception = false;
			Q2.extend = true;
			Aatrox.spells.emplace_back(Q2);

			Spell Q3;
			Q3.name = "AatroxQ3";
			Q3.icon = "AatroxQ3";
			Q3.displayName = "The Darkin Blade [Third]";
			Q3.missileName = "";
			Q3.slot = _Q;
			Q3.type = circular;
			Q3.speed = MathHuge;
			Q3.range = 200;
			Q3.delay = 0.6;
			Q3.radius = 300;
			Q3.angle = 0;
			Q3.danger = 4;
			Q3.cc = true;
			Q3.collision = false;
			Q3.windwall = false;
			Q3.hitbox = false;
			Q3.fow = false;
			Q3.exception = false;
			Q3.extend = false;
			Aatrox.spells.emplace_back(Q3);

			Spell W;
			W.name = "AatroxW";
			W.icon = "AatroxW";
			W.displayName = "Infernal Chains";
			W.missileName = "AatroxW";
			W.slot = _W;
			W.type = linear;
			W.speed = 1800;
			W.range = 825;
			W.delay = 0.25;
			W.radius = 80;
			W.angle = 0;
			W.danger = 2;
			W.cc = true;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Aatrox.spells.emplace_back(W);

			champs.emplace_back(Aatrox);
		}

		{
			Champ Brand;
			Brand.hero = "Brand";
			Spell Q;
			Q.name = "BrandQ";
			Q.icon = "BrandQ";
			Q.displayName = "Sear";
			Q.missileName = "BrandQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1600;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.angle = 0;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Brand.spells.emplace_back(Q);

			Spell W;
			W.name = "BrandW";
			W.icon = "BrandW";
			W.displayName = "Pillar of Flame";
			W.missileName = "BrandWMissile";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 900;
			W.delay = 0.85;
			W.radius = 250;
			W.angle = 0;
			W.danger = 2;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Brand.spells.emplace_back(W);

			champs.emplace_back(Brand);
		}

		{
			Champ Cassiopeia;
			Cassiopeia.hero = "Cassiopeia";
			Spell R;
			R.name = "CassiopeiaR";
			R.icon = "CassiopeiaR";
			R.displayName = "Petrifying Gaze";
			R.missileName = "";
			R.slot = _R;
			R.type = conic;
			R.speed = MathHuge;
			R.range = 825;
			R.delay = 0.5;
			R.radius = 790;
			R.angle = 80;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			Cassiopeia.spells.emplace_back(R);

			champs.emplace_back(Cassiopeia);
		}

		{
			Champ Diana;
			Diana.hero = "Diana";
			Spell Q;
			Q.name = "DianaQ";
			Q.icon = "DianaQ";
			Q.displayName = "Crescent Strike";
			Q.missileName = "DianaQOuterMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1900;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 185;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Diana.spells.emplace_back(Q);

			champs.emplace_back(Diana);
		}

		{
			Champ Khazix;
			Khazix.hero = "Khazix";

			Spell W1;
			W1.name = "KhazixW";
			W1.icon = "KhazixW1";
			W1.displayName = "Void Spike [Standard]";
			W1.missileName = "KhazixWMissile";
			W1.slot = _W;
			W1.type = linear;
			W1.speed = 1700;
			W1.range = 1000;
			W1.delay = 0.25;
			W1.radius = 70;
			W1.angle = 0;
			W1.danger = 1;
			W1.cc = false;
			W1.collision = true;
			W1.windwall = true;
			W1.hitbox = true;
			W1.fow = true;
			W1.exception = false;
			W1.extend = true;
			Khazix.spells.emplace_back(W1);

			Spell W2;
			W2.name = "KhazixWLong";
			W2.icon = "KhazixW2";
			W2.displayName = "Void Spike [Evolved]";
			W2.missileName = "";
			W2.slot = _W;
			W2.type = threeway;
			W2.speed = 1700;
			W2.range = 1000;
			W2.delay = 0.25;
			W2.radius = 70;
			W2.angle = 23;
			W2.danger = 2;
			W2.cc = true;
			W2.collision = true;
			W2.windwall = true;
			W2.hitbox = true;
			W2.fow = false;
			W2.exception = false;
			W2.extend = true;
			Khazix.spells.emplace_back(W2);

			champs.emplace_back(Khazix);
		}

		{
			Champ Lux;
			Lux.hero = "Lux";
			Spell Q;
			Q.name = "LuxLightBinding";
			Q.icon = "LuxQ";
			Q.displayName = "Light Binding";
			Q.missileName = "LuxLightBindingDummy";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1200;
			Q.range = 1175;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.angle = 0;
			Q.danger = 1;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Lux.spells.emplace_back(Q);

			Spell E;
			E.name = "LuxLightStrikeKugel";
			E.icon = "LuxE";
			E.displayName = "LLight Strike Kugel";
			E.missileName = "LuxLightStrikeKugel";
			E.slot = _E;
			E.type = circular;
			E.speed = 1200;
			E.range = 1100;
			E.delay = 0.25;
			E.radius = 300;
			E.angle = 0;
			E.danger = 3;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Lux.spells.emplace_back(E);

			Spell R;
			R.name = "LuxMaliceCannonMis";
			R.icon = "LuxR";
			R.displayName = "Malice Cannon";
			R.missileName = "LuxRVfxMis";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 3340;
			R.delay = 1;
			R.radius = 120;
			R.angle = 0;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Lux.spells.emplace_back(R);

			champs.emplace_back(Lux);
		}

		{
			Champ Morgana;
			Morgana.hero = "Morgana";
			Spell Q;
			Q.name = "MorganaQ";
			Q.icon = "MorganaQ";
			Q.displayName = "Dark Binding";
			Q.missileName = "MorganaQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1200;
			Q.range = 1250;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.angle = 0;
			Q.danger = 1;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Morgana.spells.emplace_back(Q);

			champs.emplace_back(Morgana);
		}

		{
			Champ TwistedFate;
			TwistedFate.hero = "TwistedFate";

			Spell Q;
			Q.name = "WildCards";
			Q.icon = "TwistedFateQ";
			Q.displayName = "Wild Cards";
			Q.missileName = "SealFateMissile";
			Q.slot = _Q;
			Q.type = threeway;
			Q.speed = 1000;
			Q.range = 1450;
			Q.delay = 0.25;
			Q.radius = 40;
			Q.angle = 28;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			TwistedFate.spells.emplace_back(Q);
			champs.emplace_back(TwistedFate);
		}

		{
			Champ Zilean;
			Zilean.hero = "Zilean";

			Spell Q;
			Q.name = "ZileanQ";
			Q.icon = "ZileanQ";
			Q.displayName = "Time Bomb";
			Q.missileName = "ZileanQMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 900;
			Q.delay = 0.8;
			Q.radius = 150;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Zilean.spells.emplace_back(Q);
			champs.emplace_back(Zilean);
		}
	}

	std::string SlotToName(int slot)
	{
		switch (slot)
		{
		case 0:
			return "[Q]";
		case 1:
			return "[W]";
		case 2:
			return "[E]";
		case 3:
			return "[R]";
		case 4:
			return "[D]";
		case 5:
			return "[F]";
		}
	}

	void GUI()
	{
		ImGui::SetNextWindowPos(ImVec2(548, 189), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(361, 377), ImGuiCond_FirstUseEver);

		ImGui::Begin("Spells usage");

		for (auto& a : evadespelldb)
		{
			if (ImGui::CollapsingHeader(a.hero.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (auto& s : a.spells)
				{
					std::string str = "Use ##" + s.name;
					ImGui::Text("%s - %s", SlotToName(s.slot).c_str(), s.displayName.c_str());
					ImGui::Checkbox(str.c_str(), &s.use);
					std::string str2 = "Danger ##" + s.name;
					ImGui::SliderInt(str2.c_str(), &s.danger, 1, 5);
					ImGui::Separator();
				}
			}
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(429, 189), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(361, 377), ImGuiCond_FirstUseEver);

		ImGui::Begin("Spells to dodge");

		for (auto& a : champs)
		{
			if (ImGui::CollapsingHeader(a.hero.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (auto& s : a.spells)
				{
					std::string str = "Dodge ##" + s.name;
					ImGui::Text("%s - %s", SlotToName(s.slot).c_str(), s.displayName.c_str());
					ImGui::Checkbox(str.c_str(), &s.dodge);
					std::string str2 = "Danger ##" + s.name;
					ImGui::SliderInt(str2.c_str(), &s.danger, 1, 5);
					ImGui::Separator();
				}
			}
		}

		ImGui::End();
	}

	Vector3 CalculateEndPos(Vector3 startPos1, Vector3 placementPos, Vector3 unitPos, Spell& spellInfo)
	{
		float range = spellInfo.range;
		Vector3 endPos = startPos1.Extend(placementPos, range);

		if (!spellInfo.extend)
		{
			if (range > 0)
			{
				if (unitPos.Distance(placementPos) < range)
				{
					endPos = placementPos;
				}
			}
			else
			{
				endPos = unitPos;
			}
		}
		else if (spellInfo.type == linear)
		{
			if (spellInfo.speed != MathHuge)
			{
				endPos = endPos.Append(startPos1, endPos, spellInfo.radius);
			}
			if (spellInfo.collision)
			{
				Vector3 startPos = startPos1.Extend(placementPos, 45);
				std::vector<Vector3>minions;
				for (CObject& minion : g_MinionList)
				{
					if (minion.GetNetworkID() - (unsigned int)0x40000000 > 0x100000)
						continue;
					if (minion.Address() && minion.GetTeam() == Local.GetTeam() && minion.HasUnitTags(Unit_Minion_Lane))
					{
						Vector3 minionPos = minion.GetPosition();
						if (minionPos.Distance(startPos) <= range && minion.GetMaxHealth() > 295 && minion.GetHealth() > 5)
						{
							auto col = minionPos.ProjectOn(startPos, placementPos).SegmentPoint;
							if (!col.IsZero() && col.Distance(minionPos) < ((minion.GetBoundingRadius()) / 2 + spellInfo.radius))
							{
								minions.emplace_back(minionPos);
							}
						}
					}
				}
				if (minions.size() > 0)
				{
					std::sort(minions.begin(), minions.end(), [&](Vector3 const& a, Vector3 const& b)
						{
							return a.DistanceSquared(startPos) < b.DistanceSquared(startPos);
						});
					float range2 = startPos.Distance(minions.front());
					endPos = (startPos).Extend(placementPos, range2);
					spellInfo.range = range2;
				}
			}
		}

		/*return endPos, not extend and
			self:Distance(startPos, endPos) or range*/

		if (!spellInfo.extend)
			spellInfo.range = startPos1.Distance(endPos);

		return endPos;
	}

	void GetPaths(Vector3& startPos, Vector3& endPos, Spell& spellInfo, CObject& unit, Geometry::Polygon& path1, Geometry::Polygon& path2)
	{
		if (spellInfo.type == linear || spellInfo.type == polygon)
		{
			path1 = Geometry::Rectangle(startPos, endPos, spellInfo.radius).ToPolygon(Bounding);
			path2 = Geometry::Rectangle(startPos, endPos, spellInfo.radius).ToPolygon();
		}
		else if (spellInfo.type == rectangular)
		{
			Vector3 dir = (endPos - startPos).Perpendicular().Normalized() * (/*data.radius2 or*/ 400);
			Vector3 startPos2 = endPos - dir;
			Vector3 endPos2 = endPos + dir;

			path1 = Geometry::Rectangle(startPos2, endPos2, spellInfo.radius / 2).ToPolygon(Bounding);
			path2 = Geometry::Rectangle(startPos2, endPos2, spellInfo.radius / 2).ToPolygon();
		}
		else if (spellInfo.type == circular)
		{
			path1 = Geometry::Circle(endPos, spellInfo.radius + Bounding).ToPolygon();
			path2 = Geometry::Circle(endPos, spellInfo.radius).ToPolygon();
		}
		else if (spellInfo.type == conic)
		{
			path1 = Geometry::Sector(startPos, endPos - startPos, spellInfo.angle * M_PI / 180.0f, spellInfo.range).ToPolygon(Bounding);
			path2 = Geometry::Sector(startPos, endPos - startPos, spellInfo.angle * M_PI / 180.0f, spellInfo.range).ToPolygon();
		}
		else if (spellInfo.type == threeway)
		{
			path1 = Geometry::Rectangle(startPos, endPos, spellInfo.radius).ToPolygon(Bounding);
			path2 = Geometry::Rectangle(startPos, endPos, spellInfo.radius).ToPolygon();
		}
	}

	bool IsDangerous(Vector3 pos)
	{
		for (auto& a : DetectedSpells)
		{
			if (a.path.IsInside(pos))
				return true;
		}
		return false;
	}

	float GetMovementSpeed(bool extra, EvadeSpell evadeSpell)
	{
		float moveSpeed = Local.GetMS();
		if (!extra)
			return moveSpeed;
		if (evadeSpell.type == 0)
			return 9999;
		CSpellSlot* slot;
		slot = Local.GetSpellByID(evadeSpell.slot);
		auto lvl = slot->GetLevel();
		auto name = evadeSpell.name;
		if (!lvl || lvl == 0)
			return moveSpeed;
		if (name == "AnnieE")
			return (1.2824 + (0.0176 * Local.GetLevel())) * moveSpeed;
		else if (name == "BlitzcrankW")
		{
			float speed[] = { 1.7, 1.75, 1.8, 1.85, 1.9 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "DravenW")
		{
			float speed[] = { 1.4, 1.45, 1.5, 1.55, 1.6 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "GarenQ")
			return 1.3 * moveSpeed;
		else if (name == "KaisaE")
		{
			float speed[] = { 1.55, 1.6, 1.65, 1.7, 1.75 };
			return speed[lvl] * 2 * min(1, Local.GetBonusAS()) * moveSpeed;//todo maybe full as
		}
		else if (name == "KatarinaW")
		{
			float speed[] = { 1.5, 1.6, 1.7, 1.8, 1.9 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "KennenE")
			return 2 * moveSpeed;
		else if (name == "RumbleW")
		{
			float speed[] = { 1.2, 1.25, 1.3, 1.35, 1.4 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "ShyvanaW")
		{
			float speed[] = { 1.3, 1.35, 1.4, 1.45, 1.5 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "SkarnerW")
		{
			float speed[] = { 1.08, 1.1, 1.12, 1.14, 1.16 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "SonaE")
		{
			float speed[] = { 1.1, 1.11, 1.12, 1.13, 1.14 };
			return (speed[lvl] + Local.GetAP() / 100 * 0.03) * moveSpeed;
		}
		else if (name == "TeemoW")
		{
			float speed[] = { 1.1, 1.14, 1.18, 1.22, 1.26 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "UdyrE")
		{
			float speed[] = { 1.15, 1.2, 1.25, 1.3, 1.35, 1.4 };
			return speed[lvl] * moveSpeed;
		}
		else if (name == "VolibearQ")
		{
			float speed[] = { 1.15, 1.175, 1.2, 1.225, 1.25 };
			return speed[lvl] * moveSpeed;
		}

		return moveSpeed;
	}

	bool IsAboutToHit(DetectedSpell& spell, Vector3 pos1, int extra)
	{
		EvadeSpell evadeSpell;
		for (auto e : evadespelldb)
			if (utils->ToLower(e.hero) == utils->ToLower(Local.GetChampName()))
			{
				for (auto es : e.spells)
				{
					if (es.type == 2 && es.use)
						evadeSpell = es;
				}
			}
		if (extra && evadeSpell.type != 2) //movement speed buff
			return false;
		float moveSpeed = GetMovementSpeed(extra, evadeSpell);
		Vector3 myPos = MyHeroPos;
		float diff = GameTimer - spell.startTime;
		Vector3 pos = myPos.Append(myPos, pos1, 99999);
		if (spell.spell.speed != MathHuge && spell.spell.type == linear || spell.spell.type == threeway)
		{
			if (spell.spell.delay > 0 && diff <= spell.spell.delay)
			{
				myPos = (myPos).Extend(pos, (spell.spell.delay - diff) * moveSpeed);
				if (!spell.path.IsInside(myPos))
					return false;
				auto va = (pos - myPos).Normalized() * moveSpeed;
				auto vb = (spell.endPos - spell.position).Normalized() * spell.spell.speed;
				auto da = (myPos - spell.position);
				auto db = (va - vb);
				auto a = db.DotProduct(db);
				auto b = 2 * da.DotProduct(db);
				auto c = da.DotProduct(da) - std::pow((spell.spell.radius + Bounding * 2), 2);
				auto delta = b * b - 4 * a * c;
				if (delta >= 0)
				{
					auto rtDelta = std::sqrtf(delta);
					auto t1 = (-b + rtDelta) / (2 * a);
					auto t2 = (-b - rtDelta) / (2 * a);

					return max(t1, t2) >= 0;
				}
				return false;
			}
		}
		auto t = max(0, spell.spell.range / spell.spell.speed + spell.spell.delay - diff - 0.07);
		return spell.path.IsInside(myPos.Extend(pos, moveSpeed * t));
	}

	bool IsSafePos(Vector3 pos, int extra)
	{
		for (DetectedSpell& s : DodgeableSpells)
		{
			if (s.path.IsInside(pos) || IsAboutToHit(s, pos, extra))
				return false;
		}
		return true;
	}

	void SpellExistsThenRemove(std::string name)
	{
		for (int i = 0; i < DetectedSpells.size(); i++)
		{
			auto s = DetectedSpells[i];
			if (name == s.spell.name)
				DetectedSpells.erase(DetectedSpells.begin() + i);
		}
	}

	bool ValidTarget(CObject target, float range = MathHuge)
	{
		if (target.Address() != 0 && !target.IsDead())
		{
			return MyHeroPos.DistanceSquared(target.GetPosition()) <= range * range;
		}
		return false;
	}

	Vector3 GetBestEvadePos(std::vector<DetectedSpell> spells, float radius, int mode, int extra, bool force)
	{
		std::vector<Vector3> points;

		std::vector<Geometry::Polygon> polygons;

		for (auto const& spell : spells)
		{
			polygons.push_back(spell.path);
			//draw->Polygon(spell.path, RGBA(255, 255, 0));
		}

		//clipping polygons moves them?
		auto danger_polygons = polygons;
		//auto danger_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(polygons));

		//Scan the sides of each polygon to find the safe point.
		for (Geometry::Polygon poly : danger_polygons)
		{
			//draw->Polygon(poly, RGBA(COLOR_YELLOW));
			for (size_t i = 0; i < poly.Points.size(); ++i)
			{
				auto startPos = poly.Points[i];
				auto endPos = poly.Points[(i == poly.Points.size() - 1) ? 0 : i + 1];

				auto my_position = MyHeroPos;
				auto original = my_position.ProjectOn(startPos, endPos).SegmentPoint;

				auto distSqr = original.DistanceSquared(my_position);

				if (distSqr <= 360000)
				{
					if (force)
					{
						auto candidate = MyHeroPos.Append(MyHeroPos, original, 5);
						if (distSqr && !IsDangerous(candidate) && !InWall(candidate))
							points.push_back(candidate);
					}
					else
					{
						auto side_distance = endPos.DistanceSquared(startPos);
						auto direction = (endPos - startPos).Normalized();
						int step = (distSqr < 200 * 200 && side_distance > 90 * 90) ? M.Evade.DC : 0;

						for (int j = -step; j <= step; j++)
						{
							auto candidate = original + direction * (j * M.Evade.DS);
							Vector3 extended = MyHeroPos.Append(MyHeroPos, candidate, Bounding);
							candidate = MyHeroPos.Append(MyHeroPos, candidate, 5);

							if (IsSafePos(candidate, extra) && !InWall(candidate))
							{
								points.push_back(candidate);
								if (M.bDebug)
									draw->CircleRange(ExtendedPos, 16, Bounding, RGBA(255, 255, 255));
							}
						}
					}
				}
			}
		}
		if (points.size() > 0)
		{
			Vector3 tempMyHeroPos = MyHeroPos;
			float tempRB = radius + Bounding;
			Vector3 tempMPos = mouse->GetWorldPos();
			//todo
			//for no reason when we are stuck, sometimes it sometimes throws invalid comparator
			//so for now we just ignore it
			try
			{
				if (mode == 1)
				{
					std::sort(points.begin(), points.end(), [&tempMyHeroPos](Vector3 const& a, Vector3 const& b)
						{
							return a.DistanceSquared(tempMyHeroPos) < b.DistanceSquared(tempMyHeroPos);
						});
				}
				else if (mode == 2)
				{
					std::sort(points.begin(), points.end(), [&tempMyHeroPos, &tempRB, &tempMPos](Vector3 const& a, Vector3 const& b)
						{
							Vector3 mPos = tempMyHeroPos.Extend(tempMPos, tempRB);
							return a.DistanceSquared(mPos) < b.DistanceSquared(mPos);
						});
				}
			}
			catch (std::exception const&) {}
			return points.front();
		}

		return 0;
	}

	Vector3 GetExtendedSafePos(Vector3 pos)
	{
		if (!M.Evade.Smooth)
			return pos;
		float distance = MyHeroPos.Distance(pos);
		std::vector<Vector3>positions;
		for (CObject& minion : g_MinionList)
		{
			if (minion.GetNetworkID() - (unsigned int)0x40000000 > 0x100000)
				continue;
			if (minion.Address() && minion.HasUnitTags(Unit_Minion_Lane) && !minion.IsDead())
			{
				Vector3 minionPos = minion.GetPosition();
				if (MyHeroPos.Distance(minionPos) <= distance)
				{
					positions.emplace_back(minionPos);
				}
			}
		}
		for (int i = 1; i < 8; i++)
		{
			bool collision = false;
			auto ext = MyHeroPos.Append(MyHeroPos, pos, Bounding * i);
			if (i > 1 && InWall(ext) || i == 1)
			{
				for (auto& minionPos : positions)
				{
					if (ext.Distance(minionPos) <= Bounding)
					{
						collision = true;
						break;
					}
				}
				if (!collision)
					return ext;
			}
		}
		return 0;
	}
	int SlotToScancode(int slot)
	{
		switch (slot)
		{
		case 0:
			return DIK_Q;
		case 1:
			return DIK_W;
		case 2:
			return DIK_E;
		case 3:
			return DIK_R;
		case 4:
			return DIK_D;
		case 5:
			return DIK_F;
		default:
			return 0;
		}
		return 0;
	}

	void CastSpell(int slot, Vector3 pos)
	{
		if (!draw->IsOnScreen(pos))
			return;

		POINT startPos = mouse->GetPos();
		ImVec2 path = Direct3D9.WorldToScreen(pos);
		mouse->MouseMoveInstant(path.x, path.y);
		keyboard->GenerateKeyScancode(SlotToScancode(slot));
		mouse->MouseMoveInstant(startPos.x, startPos.y);
	}

	int Avoid(DetectedSpell spell, Vector3 dodgePos, EvadeSpell data)
	{
		CSpellSlot* slot;
		slot = Local.GetSpellByID(data.slot);
		if (slot->IsReady(Local.GetMana()))
		{
			if (!dodgePos.IsZero() && !IsDangerous(dodgePos) && (data.type == 1 || data.type == 2))
			{
				if (data.type == 1) //dash
				{
					Vector3 dashPos = MyHeroPos.Extend(dodgePos, data.range);
					CastSpell(data.slot, dashPos);
					return 1;
				}
				else if (data.type == 2) //ms buff
				{
					CastSpell(data.slot, MyHeroPos);
					return 1;
				}
			}
			else if (data.type == 3) //untargetble fizz e vlad w xayah r
			{
				CastSpell(data.slot, MyHeroPos);
				return 2;
			}
			else if (data.type == 4)//yi q zed r
			{
				for (auto hero : init->herolist)
				{
					if (hero.GetTeam() != Local.GetTeam() && hero.IsVisible() && !hero.IsDead())
					{
						CastSpell(data.slot, hero.GetPosition());
						return 2;
					}
				}
			}
			else if (data.type == 5 && spell.spell.cc) //spellshield
			{
				CastSpell(data.slot, MyHeroPos);
				return 2;
			}
			//else if 6 //widnwall
			else if (data.type == 7 && spell.spell.cc) //fiora parry
			{
				CastSpell(data.slot, spell.position);
				return 2;
			}
		}

		return 0;
	}

	void MoveToPos(Vector3 pos, bool stop = false)
	{
		if (!draw->IsOnScreen(pos))
			return;
		if (!Local.CheckState(CanMove))
			return;
		if (!IsSafePos(pos, false))
			return;
		POINT startPos = mouse->GetPos();
		ImVec2 path = Direct3D9.WorldToScreen(pos);
		mouse->MouseMoveInstant(path.x, path.y);
		bool over = false;
		if (Memory.Read<DWORD>(ClientAddress + oUnderMouseObject) != 0) //if smth under mouse
		{
			over = true;
			keyboard->GenerateKeyScancode(DIK_Z);
		}

		mouse->RightClick();
		if (over)
			keyboard->GenerateKeyScancode(DIK_Z);
		if (M.Evade.MouseBack || stop)
			mouse->MouseMoveInstant(startPos.x, startPos.y);
	}

	int CoreManager(DetectedSpell s)
	{
		if (s.path.IsInside(MyHeroPos) && Local.CheckState(CanMove))
		{
			if (OldTimer != NewTimer)
			{
				std::vector<EvadeSpell> evadeSpells;
				for (auto e : evadespelldb)
					if (e.hero == Local.GetChampName())
					{
						for (auto es : e.spells)
						{
							if (es.use)
								evadeSpells.emplace_back(es);
						}
					}
				/*	local evadeSpells = self.EvadeSpellData
						local flashUsage = self.Flash2 and self.JEMenu.Spells.Flash.US:Value()
						and self : IsReady(self.Flash2) and s.danger == 5*/
				Vector3 safePos = GetBestEvadePos(DodgeableSpells, s.spell.radius, 2, 0, false);
				int result = 0;
				if (!safePos.IsZero())
				{
					ExtendedPos = GetExtendedSafePos(safePos);
					SafePos = safePos;
					Evading = true;
				}
				else if (evadeSpells.size() > 0)//|| flashUsage)
				{
					for (int i = 0; i < evadeSpells.size(); i++)
					{
						Vector3 alternPos = GetBestEvadePos(DodgeableSpells, s.spell.radius, 1, i + 1, false);
						result = Avoid(s, alternPos, evadeSpells[i]);
						if (result > 0)
						{
							if (result == 1)
							{
								ExtendedPos = GetExtendedSafePos(alternPos);
								SafePos = alternPos;
								Evading = true;
							}
							break;
						}
					}
				}
				else
				{
					if (result == 0)
					{
						Vector3 dodgePos = GetBestEvadePos(DodgeableSpells, s.spell.radius, 1, true, true);
						if (!dodgePos.IsZero())
						{
							/*if (flashUsage)
							{
								result = 1;
								CastSpell(Flash, DodgePos);
							}*/
							if (M.Evade.Force)
							{
								clog.AddLog("Forced dodge");
								ExtendedPos = GetExtendedSafePos(dodgePos);
								SafePos = dodgePos;
								Evading = true;
							}
						}
					}
					if (result == 0)
						clog.AddLog("Impossible to dodge");
					//impossible dodge
					//for i = 1, #self.OnImpDodgeCBs do self.OnImpDodgeCBs[i](s.danger) end
				}

				OldTimer = NewTimer;
			}

			return 1;
		}
		return 0;
	}

	void OnProcessSpell(CObject& unit, CSpellEntry& spell, Spell& info)
	{
		if (!unit.Address() || !spell.Address())
			return;

		if (M.bDebug || unit.GetTeam() != Local.GetTeam())
		{
			if (M.Evade.LimitRange && Local.GetDistTo(unit) > M.Evade.LR)
				return;

			Vector3 startPos = spell.GetMissileStartPos();
			//clog.AddLog("proc %f %s", startPos.y,spell.spellInfo->name.c_str());
			Vector3 endPos = spell.GetMissileEndPos();
			Vector3 unitPos = unit.GetPosition();
			Vector3 RealEndPos = CalculateEndPos(startPos, endPos, unitPos, info);
			Geometry::Polygon path1;
			Geometry::Polygon path2;
			GetPaths(startPos, RealEndPos, info, unit, path1, path2);

			if (path1.Points.empty())
				return;
			Spell tinfo = info;
			AddSpell(path1, path2, startPos, RealEndPos, info);
			if (info.type == threeway)
			{
				for (int i = 0; i < 2; i++)
				{
					Vector3 eP;
					if (i == 0)
					{
						eP = RealEndPos.Rotate(startPos, DEG2RAD(info.angle));
					}
					else if (i == 1)
					{
						eP = RealEndPos.Rotate(startPos, -DEG2RAD(info.angle));
					}
					auto p1 = Geometry::Rectangle(startPos, eP, info.radius).ToPolygon(Bounding);
					auto p2 = Geometry::Rectangle(startPos, eP, info.radius).ToPolygon();
					AddSpell(p1, p2, startPos, eP, info);
				}
			}

			NewTimer = GameTimer;
		}
		else if (unit.Address() == Local.Address() && (spell.spellInfo->name == "SummonerFlash"))
		{
			NewTimer = GameTimer;
			SafePos = 0;
			ExtendedPos = 0;
		}
	}

	void OnCreateMissile(CObject& unit, CMissileClient& missile, Spell& info)
	{
		if (M.Evade.LimitRange && Local.GetDistTo(unit) > M.Evade.LR)
			return;

		missile.spellInfo->delay = 0;
		Vector3 startPos = missile.GetMissileStartPos();
		//clog.AddLog("mis %f", startPos.y);
		Vector3 endPos = missile.GetMissileEndPos();
		Vector3 unitPos = unit.GetPosition();
		Vector3 RealEndPos = CalculateEndPos(startPos, endPos, unitPos, info);
		Geometry::Polygon path1;
		Geometry::Polygon path2;
		GetPaths(startPos, RealEndPos, info, unit, path1, path2);

		if (path1.Points.empty())
			return;

		AddSpell(path1, path2, startPos, RealEndPos, info);
		/*if (info.type == threeway)
		{
			for (int i = 0; i < 2; i++)
			{
				Vector3 eP;
				if (i == 0)
				{
					eP = endPos.Rotate(startPos, DEG2RAD(info.angle));
				}
				else if (i == 1)
				{
					eP = endPos.Rotate(startPos, -DEG2RAD(info.angle));
				}
				auto p1 = Geometry::Rectangle(startPos, eP, info.radius).ToPolygon(Bounding);
				auto p2 = Geometry::Rectangle(startPos, eP, info.radius).ToPolygon();
				AddSpell(p1, p2, startPos, eP, info);
			}
		}*/

		NewTimer = GameTimer;
	}

	void DodgeSpell()
	{
		//if is channeling
		/*if Buffsand Buffs[myHero.charName] and
			self:HasBuff(Buffs[myHero.charName]) then
			self.SafePos, self.ExtendedPos = nil, nil
			end*/

		if (!ExtendedPos.IsZero())
		{
			MoveToPos(ExtendedPos);
		}
	}

	Vector3 GetMovePath()
	{
		if (!Local.IsMoving())
			return 0;
		return Local.GetAiManager()->GetNavEnd();
	}

	std::vector<Vector3> FindIntersections(Geometry::Polygon poly, Vector3 p1, Vector3 p2)
	{
		std::vector<Vector3>intersections;
		for (size_t i = 0; i < poly.Points.size(); ++i)
		{
			auto startPos = poly.Points[i];
			auto endPos = poly.Points[(i == poly.Points.size() - 1) ? 0 : i + 1];
			auto temp = startPos.Intersection(endPos, p1, p2).Point;
			if (!temp.IsZero())
				intersections.emplace_back(temp);
		}
		return intersections;
	}

	Vector3 PrependVector(Vector3 pos1, Vector3 pos2, float dist)
	{
		return pos1 + (pos2 - pos1).Normalized() * dist;
	}

	void Tick()
	{
		if (!M.Evade.Master || M.fGameTime < 5.f || M.bMenuOpen)//|| !utils->IsLeagueForeGround())
			return;

		if (M.Evade.OnKey && !PressedKey(M.Evade.EvadeKey))
			return;

		//	clog.AddLog("detected: %d dodgeable: %d", DetectedSpells.size(), DodgeableSpells.size());

			//self.DoD = self.JEMenu.Main.DD:Value() == true
		if (Local.IsDead())
			return;
		//CObject Local(0x4a4308f0);
		MyHeroPos = Local.GetPosition();
		Bounding = Local.GetBoundingRadius() + 5.f;
		GameTimer = M.fGameTime;

		//spell entry
		for (auto& hero : init->herolist)
		{
			if (hero.IsDead() && hero.IsVisible())
				continue;

			if (CSpellEntry spell(hero.GetActiveSpellEntry()); spell.Address() != 0)
			{
				/*	DWORD addr = Memory.Read<DWORD>(Memory.Read<DWORD>(spell.Address() + oSpellEntrySpellInfo) + oSpellInfoSpellData);
					for (int i = 0; i < 10000; i += 4)
					{
						clog.AddLog("0x%x = f%f , d%d , %s", i, Memory.Read<float>(addr + i), Memory.Read<int>(addr + i), Memory.ReadString(addr + i));
					}*/

					//clog.AddLog("ps%s", spell.spellInfo->name.c_str());
					//clog.AddLog("ps%s", spell.GetSpellInfo()->GetName().c_str());

				std::string heroName = hero.GetChampName();
				for (auto h : champs)
				{
					//if hero in db
					if (h.hero == heroName)
					{
						for (auto s : h.spells)
						{
							bool detected = false;
							//if spell in db
							if (utils->ToLower(s.name) == utils->ToLower(spell.spellInfo->name) || utils->ToLower(s.missileName) == utils->ToLower(spell.spellInfo->name))
							{
								for (auto a : DodgeableSpells)
									if (a.startPos == spell.GetMissileStartPos() && utils->ToLower(a.spell.name) == utils->ToLower(spell.spellInfo->name))
									{
										detected = true;
										break;
									}
								//if spell wasnt already detected
								if (!detected)
								{
									if (s.dodge)
										OnProcessSpell(hero, spell, s);
									break;
								}
							}
						}
						break;
					}
				}
			}
		}

		//missile map
		if (M.Evade.Missile)
		{
			int numMissiles = Memory.Read<int>(MissileMap + 0x78);
			int rootNode = Memory.Read<int>(MissileMap + 0x74);
			std::queue<int> nodesToVisit;
			std::set<int> visitedNodes;
			nodesToVisit.push(rootNode);

			int childNode1, childNode2, childNode3, node;
			while (nodesToVisit.size() > 0 && visitedNodes.size() < numMissiles * 2)
			{
				node = nodesToVisit.front();
				nodesToVisit.pop();
				visitedNodes.insert(node);
				//clog.AddLog("%x", node);
				childNode1 = Memory.Read<int>(node);
				childNode2 = Memory.Read<int>(node + 4);
				childNode3 = Memory.Read<int>(node + 8);

				if (visitedNodes.find(childNode1) == visitedNodes.end())
					nodesToVisit.push(childNode1);
				if (visitedNodes.find(childNode2) == visitedNodes.end())
					nodesToVisit.push(childNode2);
				if (visitedNodes.find(childNode3) == visitedNodes.end())
					nodesToVisit.push(childNode3);

				unsigned int netId = Memory.Read<int>(node + 0x10);

				// Actual missiles net_id start from 0x40000000 and throught the game this id will be incremented by 1 for each missile.
				// So we use this information to check if missiles are valid.
				if (netId - (unsigned int)0x40000000 > 0x100000)
					continue;

				int addr = Memory.Read<int>(node + 0x14);
				if (addr == 0)
					continue;

				// The MissileClient is wrapped around another object
				addr = Memory.Read<int>(addr + 0x4);
				if (addr == 0)
					continue;

				addr = Memory.Read<int>(addr + 0x10);
				if (addr == 0)
					continue;

				CMissileClient obj(addr);

				// Check one more time that we read a valid missile
				if (obj.GetNetworkID() != netId)
					continue;

				int team = obj.GetTeam();
				if (team == 100 || team == 200 || team == 300)
				{
					if (!M.bDebug && team == Local.GetTeam())
						continue;
					CObject source = obj.GetSource();
					if (source.IsHero())
					{
						//clog.AddLog("%x", obj.Address());
						if (!utils->StringContains(obj.GetName(), "basic", true))
						{
							/*for (int i = oMissileSpellInfo; i < 10000; i += 4)
							{
								clog.AddLog("0x%x = f%f , d%d , %s", i, Memory.Read<float>(obj.Address() + i), Memory.Read<int>(obj.Address() + i), Memory.ReadString(obj.Address() + i));
							}*/

							//clog.AddLog("ms%s", obj.spellInfo->name.c_str());

							//if spell exists in database
							std::string heroName = source.GetChampName();
							for (auto h : champs)
							{
								//if hero in db
								if (h.hero == heroName)
								{
									for (auto s : h.spells)
									{
										bool detected = false;
										//if spell in db
										if (utils->ToLower(s.name) == obj.spellInfo->name || utils->ToLower(s.missileName) == obj.spellInfo->name)
										{
											for (auto a : DodgeableSpells)
												if (a.startPos == obj.GetMissileStartPos() && (utils->ToLower(a.spell.name) == utils->ToLower(obj.spellInfo->name) || utils->ToLower(a.spell.missileName) == utils->ToLower(obj.spellInfo->name)))
												{
													detected = true;
													break;
												}
											//if spell wasnt already detected
											if (!detected)
											{
												if (s.dodge)
													OnCreateMissile(source, obj, s);
												break;
											}
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		}

		if (DodgeableSpells.size() > 0)
		{
			int result = 0;
			for (DetectedSpell& spell : DodgeableSpells)
				result += CoreManager(spell);

			Vector3 movePath = GetMovePath();
			if (!movePath.IsZero() && !Evading)
			{
				std::vector<Vector3> ints;
				for (auto s : DodgeableSpells)
				{
					auto poly = s.path;
					if (!poly.IsInside(MyHeroPos))
					{
						ints = FindIntersections(poly, MyHeroPos, movePath);
					}
				}
				if (ints.size() > 0)
				{
					std::sort(ints.begin(), ints.end(), [&](Vector3 const& a, Vector3 const& b)
						{
							return MyHeroPos.DistanceSquared(a) < MyHeroPos.DistanceSquared(b);
						});
					auto movePos = PrependVector(MyHeroPos, ints.front(), Bounding / 2);
					MoveToPos(movePos, true);
				}
			}

			if (Evading)
				DodgeSpell();
			if (result == 0)
			{
				Evading = false;
				SafePos = 0;
				ExtendedPos = 0;
			}
		}
	}

	bool SpellManager(DetectedSpell& s, int i)
	{
		if ((s.startTime + s.spell.range / s.spell.speed + s.spell.delay) > GameTimer)
		{
			if (s.spell.speed != MathHuge && s.startTime + s.spell.delay < GameTimer)
			{
				if (s.spell.type == linear || s.spell.type == threeway)
				{
					float rng = s.spell.speed * (GameTimer - s.startTime - s.spell.delay);
					Vector3 sP = s.startPos.Extend(s.endPos, rng);
					s.position = sP;
					s.path = Geometry::Rectangle(sP, s.endPos, s.spell.radius).ToPolygon(Bounding);
					s.path2 = Geometry::Rectangle(sP, s.endPos, s.spell.radius).ToPolygon();
				}
				//clog.AddLog("%f", s.spell.speed);
			}
		}
		else
		{
			DetectedSpells.erase(DetectedSpells.begin() + i);
			return true;
		}
		return false;
	}

	std::vector<DetectedSpell> GetDodgeableSpells()
	{
		std::vector<DetectedSpell> result;
		int size = DetectedSpells.size();
		for (int i = 0; i < size; i++)
		{
			if (SpellManager(DetectedSpells[i], i))
			{
				size--;
				if (i >= 0)
					i--;
			}
			else
				result.emplace_back(DetectedSpells[i]);
		}
		return result;
	}

	void Draw()
	{
		if (!M.Evade.Master)
			return;
		DodgeableSpells = GetDodgeableSpells();

		//if (DetectedSpells.size() > 0)
		//{
		for (auto a : DodgeableSpells)
		{
			//clog.AddLog("%s", a.spell->displayName);
			if (M.Evade.Draw)
			{
				draw->Polygon(a.path, RGBA(0, 255, 0));
				draw->Polygon(a.path2, RGBA(255, 0, 0));
			}

			if (!ExtendedPos.IsZero())
			{
				draw->CircleRange(ExtendedPos, 16, Bounding, RGBA(255, 80, 0));
			}
			if (!SafePos.IsZero())
			{
				draw->CircleRange(SafePos, 16, Bounding, RGBA(0, 255, 0));
				//draw->Line(Direct3D9.WorldToScreen(MyHeroPos), Direct3D9.WorldToMinimap(ExtendedPos), RGBA(255, 255, 255));
			}
		}

		if (M.bDebug)
		{
			for (CObject& obj : init->herolist)
			{
				draw->CircleRange(obj.GetPosition(), 16, obj.GetBoundingRadius(), RGBA(255, 255, 255));
			}
		}
	}
};

#endif // !_EVADE_H_
