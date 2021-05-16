#include "Pantheon.h"
#include "DirectX.h"

void Pantheon::OnKeyDown(int)
{
}

void Pantheon::OnKeyUp(int)
{
}

void Pantheon::Harass()
{
}

bool Pantheon::Evade(Vector3 evadePos)
{
	return false;
}

void Pantheon::Tick()
{
	if (M.Pantheon.DrawExecute)
	{
		for (auto& hero : init->herolist)
		{
			if (hero.IsDead() || !hero.IsVisible() || (Local.GetTeam() == hero.GetTeam()))
				continue;
			ImVec2 p = Direct3D9.GetHpBarPos(hero);
			p.y -= 25;
			//-47 is start of hp bar +58 is end of hp bar, +12 is bottom height
			//hp bar is 105 pixels width, so 20% is at 21th pixel, -47+21
			draw->Line(ImVec2(p.x - 26, p.y), ImVec2(p.x - 26, p.y + 12), RGBA(200, 0, 255), 3.f);
		}
		/*for (auto minion : g_MinionList)
		{
			ImVec2 p = Direct3D9.GetHpBarPos(minion);
			draw->Line(ImVec2(p.x - 34, p.y),ImVec2(p.x + 32, p.y + 12), RGBA(200, 0, 255), 3.f);
		}*/
	}
}

void Pantheon::GUI()
{
	ImGui::Checkbox("Draw 20% hp threshold", &M.Pantheon.DrawExecute);
}