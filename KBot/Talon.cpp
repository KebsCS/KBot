#include "Talon.h"
#include "DirectX.h"
#include "Draw.h"
#include "Keyboard.h"
#include "Mouse.h"

void Talon::OnKeyDown(int)
{
}

void Talon::OnKeyUp(int)
{
}

void Talon::Harass()
{
}

bool Talon::Evade(Vector3 evadePos)
{
	return false;
}

void Talon::Tick()
{
	//TODO CHECKING PLAYER POSITION INSTEAD OF SLEEPS
	if (M.Talon.Master)
	{
		if (M.Talon.Jumps)
		{
			if (M.Talon.JumpsType[1])
			{
				Vector3 raptorJumpSpot = Vector3(6724.0, 48.527, 4908.0);

				if (Local.GetPosition().Distance(raptorJumpSpot) < 1000)
				{
					ImVec2 raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
					if (raptorJump.x == 0 && raptorJump.y == 0)
						return;
					draw->Circle(raptorJump.x, raptorJump.y, 75, RGBA(255, 255, 255));
					if (Local.GetPosition().Distance(raptorJumpSpot) < 400)
					{
						ImVec2 raptorJumpFinal = Direct3D9.WorldToScreen(Vector3(6190, 51.772114, 5634));
						draw->Circle(raptorJumpFinal.x, raptorJumpFinal.y, 30, RGBA(255, 255, 0));
						draw->Line(raptorJumpFinal.x, raptorJumpFinal.y, raptorJump.x, raptorJump.y, RGBA(255, 255, 0));

						if (Local.GetPosition().Distance(raptorJumpSpot) < 155 && PressedKey(VK_LSHIFT))
						{
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Direct3D9.GetViewProjectionMatrix();
							raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();

							ImVec2 raptorJumpCorrection = Direct3D9.WorldToScreen(Vector3(6786.0, 48.527, 4842.0));
							mouse->MouseMoveInstant(raptorJumpCorrection.x, raptorJumpCorrection.y);
							mouse->RightClick();

							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							raptorJumpCorrection = Direct3D9.WorldToScreen(Vector3(6786.0, 48.527, 4842.0));
							mouse->MouseMoveInstant(raptorJumpCorrection.x, raptorJumpCorrection.y);
							mouse->RightClick();

							raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							ImVec2 raptorJumpDestination = Direct3D9.WorldToScreen(Vector3(6324.0, 51.765816, 5808.0));
							mouse->MouseMoveInstant(raptorJumpDestination.x, raptorJumpDestination.y);
							keyboard->GenerateKeyScancode(DIK_E, false);
							mouse->MouseMoveInstant(raptorJumpFinal.x, raptorJumpFinal.y);
						}
					}
				}
			}

			if (M.Talon.JumpsType[0])
			{
				Vector3 drakeJumpSpot = Vector3(8688, 50.623039, 5196);
				if (Local.GetPosition().Distance(drakeJumpSpot) < 1000)
				{
					ImVec2 drakeJump = Direct3D9.WorldToScreen(drakeJumpSpot);
					if (drakeJump.x == 0 && drakeJump.y == 0)
						return;
					draw->Circle(drakeJump.x, drakeJump.y, 75, RGBA(255, 255, 255));
					if (Local.GetPosition().Distance(drakeJumpSpot) < 400)
					{
						ImVec2 drakeJumpFinal = Direct3D9.WorldToScreen(Vector3(9372, -71.240601, 4642));
						draw->Circle(drakeJumpFinal.x, drakeJumpFinal.y, 30, RGBA(255, 255, 0));
						draw->Line(drakeJumpFinal.x, drakeJumpFinal.y, drakeJump.x, drakeJump.y, RGBA(255, 255, 0));

						if (Local.GetPosition().Distance(drakeJumpSpot) < 155 && PressedKey(VK_LSHIFT))
						{
							mouse->MouseMoveInstant(drakeJump.x, drakeJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Direct3D9.GetViewProjectionMatrix();
							mouse->MouseMoveInstant(drakeJump.x, drakeJump.y);
							mouse->RightClick();

							ImVec2 drakeJumpCorrection = Direct3D9.WorldToScreen(Vector3(8600, 48.448730, 5214));
							mouse->MouseMoveInstant(drakeJumpCorrection.x, drakeJumpCorrection.y);
							mouse->RightClick();

							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							drakeJumpCorrection = Direct3D9.WorldToScreen(Vector3(8600, 48.448730, 5214));
							mouse->MouseMoveInstant(drakeJumpCorrection.x, drakeJumpCorrection.y);
							mouse->RightClick();

							drakeJump = Direct3D9.WorldToScreen(drakeJumpSpot);
							mouse->MouseMoveInstant(drakeJump.x, drakeJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							ImVec2 drakeJumpDestination = Direct3D9.WorldToScreen(Vector3(10048.0, -71.240601, 4408));//ImVec2(1920, 1080);//

							mouse->MouseMoveInstant(drakeJumpDestination.x, drakeJumpDestination.y);
							keyboard->GenerateKeyScancode(DIK_E, false);
							mouse->MouseMoveInstant(drakeJumpFinal.x, drakeJumpFinal.y);
						}
					}
				}
			}
			if (M.Talon.JumpsType[2])
			{
				Vector3 botJumpSpot = Vector3(12472, 51.729401, 4508);
				if (Local.GetPosition().Distance(botJumpSpot) < 800)
				{
					ImVec2 botJump = Direct3D9.WorldToScreen(botJumpSpot);
					if (botJump.x == 0 && botJump.y == 0)
						return;
					draw->Circle(botJump.x, botJump.y, 75, RGBA(255, 255, 255));
					if (Local.GetPosition().Distance(botJumpSpot) < 400)
					{
						ImVec2 botJumpFinal = Direct3D9.WorldToScreen(Vector3(13122, 51.366905, 3988));
						draw->Circle(botJumpFinal.x, botJumpFinal.y, 30, RGBA(255, 255, 0));
						draw->Line(botJumpFinal.x, botJumpFinal.y, botJump.x, botJump.y, RGBA(255, 255, 0));

						if (Local.GetPosition().Distance(botJumpSpot) < 155 && PressedKey(VK_LSHIFT))
						{
							mouse->MouseMoveInstant(botJump.x, botJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Direct3D9.GetViewProjectionMatrix();
							mouse->MouseMoveInstant(botJump.x, botJump.y);
							mouse->RightClick();

							ImVec2 botJumpDestination = Direct3D9.WorldToScreen(Vector3(13022, 51.366901, 3808));

							mouse->MouseMoveInstant(botJumpDestination.x, botJumpDestination.y);
							keyboard->GenerateKeyScancode(DIK_E, false);
							mouse->MouseMoveInstant(botJumpFinal.x, botJumpFinal.y);
						}
					}
				}
			}
			if (M.Talon.JumpsType[3])
			{
				Vector3 topJumpSpot = Vector3(2424, 54.32550, 10406);
				if (Local.GetPosition().Distance(topJumpSpot) < 800)
				{
					ImVec2 topJump = Direct3D9.WorldToScreen(topJumpSpot);
					if (topJump.x == 0 && topJump.y == 0)
						return;
					draw->Circle(topJump.x, topJump.y, 75, RGBA(255, 255, 255));
					if (Local.GetPosition().Distance(topJumpSpot) < 400)
					{
						ImVec2 topJumpFinal = Direct3D9.WorldToScreen(Vector3(1734, 52.838100, 11004));
						draw->Circle(topJumpFinal.x, topJumpFinal.y, 30, RGBA(255, 255, 0));
						draw->Line(topJumpFinal.x, topJumpFinal.y, topJump.x, topJump.y, RGBA(255, 255, 0));

						if (Local.GetPosition().Distance(topJumpSpot) < 155 && PressedKey(VK_LSHIFT))
						{
							mouse->MouseMoveInstant(topJump.x, topJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Direct3D9.GetViewProjectionMatrix();
							mouse->MouseMoveInstant(topJump.x, topJump.y);
							mouse->RightClick();

							ImVec2 topJumpDestination = Direct3D9.WorldToScreen(Vector3(1824, 52.8381, 11106));

							mouse->MouseMoveInstant(topJumpDestination.x, topJumpDestination.y);
							keyboard->GenerateKeyScancode(DIK_E, false);
							mouse->MouseMoveInstant(topJumpFinal.x, topJumpFinal.y);
						}
					}
				}
			}

			if (M.Talon.JumpsType[4])
			{
				Vector3 raptorJumpSpot = Vector3(6724.0, 48.527, 4908.0);

				if (Local.GetPosition().Distance(raptorJumpSpot) < 1000)
				{
					ImVec2 raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
					if (raptorJump.x == 0 && raptorJump.y == 0)
						return;
					draw->Circle(raptorJump.x, raptorJump.y, 75, RGBA(255, 255, 255));
					if (Local.GetPosition().Distance(raptorJumpSpot) < 400)
					{
						ImVec2 raptorJumpFinal = Direct3D9.WorldToScreen(Vector3(6190, 51.772114, 5634));
						draw->Circle(raptorJumpFinal.x, raptorJumpFinal.y, 30, RGBA(255, 255, 0));
						draw->Line(raptorJumpFinal.x, raptorJumpFinal.y, raptorJump.x, raptorJump.y, RGBA(255, 255, 0));

						if (Local.GetPosition().Distance(raptorJumpSpot) < 155 && PressedKey(VK_LSHIFT))
						{
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Direct3D9.GetViewProjectionMatrix();
							raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();

							ImVec2 raptorJumpCorrection = Direct3D9.WorldToScreen(Vector3(6786.0, 48.527, 4842.0));
							mouse->MouseMoveInstant(raptorJumpCorrection.x, raptorJumpCorrection.y);
							mouse->RightClick();

							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							raptorJumpCorrection = Direct3D9.WorldToScreen(Vector3(6786.0, 48.527, 4842.0));
							mouse->MouseMoveInstant(raptorJumpCorrection.x, raptorJumpCorrection.y);
							mouse->RightClick();

							raptorJump = Direct3D9.WorldToScreen(raptorJumpSpot);
							mouse->MouseMoveInstant(raptorJump.x, raptorJump.y);
							mouse->RightClick();
							std::this_thread::sleep_for(std::chrono::milliseconds(400));
							Direct3D9.GetViewProjectionMatrix();
							ImVec2 raptorJumpDestination = Direct3D9.WorldToScreen(Vector3(6324.0, 51.765816, 5808.0));
							mouse->MouseMoveInstant(raptorJumpDestination.x, raptorJumpDestination.y);
							keyboard->GenerateKeyScancode(DIK_E, false);
							mouse->MouseMoveInstant(raptorJumpFinal.x, raptorJumpFinal.y);
						}
					}
				}
			}
		}
	}
}

void Talon::GUI()
{
	ImGui::Checkbox("Damage Calculator", &M.Talon.DmgCalc);
	ImGui::Separator();
	ImGui::Checkbox("Pixel Perfect Jumps", &M.Talon.Jumps);
	Direct3D9.HotkeyButton(M.Talon.JumpsKey);
	ImGui::SameLine(); ImGui::Text("Jump on key");
	if (M.Talon.Jumps)
	{
		ImGui::Selectable("Drake", &M.Talon.JumpsType[0]);
		ImGui::Selectable("Blue-side Raptors", &M.Talon.JumpsType[1]);
		ImGui::Selectable("Botlane", &M.Talon.JumpsType[2]);
		ImGui::Selectable("Toplane", &M.Talon.JumpsType[3]);
		ImGui::Selectable("Red-side Raptors TODO", &M.Talon.JumpsType[4]);
	}
}