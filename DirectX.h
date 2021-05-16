#pragma once

#ifndef _DIRECTX_H_
#define _DIRECTX_H_

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <tchar.h>

#include <dinput.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <dcomp.h>
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>

//#include "CObjectManager.h"
#include "ScriptUtils.h"
#include "Config.h"
#include "Initialize.h"
#include "Visuals.h"

//#include "stb_image.h"
//#include <functional>

// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11RenderTargetView* g_pd3dRenderTargetView = NULL;
static IDXGISwapChain1* g_pSwapChain = NULL;

//static ID3D11Buffer* g_pVertexBuffer = NULL;
//static ID3D11InputLayout* g_pInputLayout = NULL;
//static ID3DX11EffectTechnique* m_pTechnique = NULL;

class Direct3D9Render
{
private:
	DirectX::XMMATRIX ReadMatrix(DWORD address);
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

	void ArrowButtonDisabled(const char* id, ImGuiDir dir);

public:
	void GetViewProjectionMatrix();

	/*int fontArial = 0;
	int fontTahoma = 0;
	int fontTahomaSmall = 0;*/

	Direct3D9Render()
	{
	}

	~Direct3D9Render() = default;

	void StartFrame();

	void EndFrame();

	// initializes directx, fonts, imgui and objects
	bool DirectXInit(HWND hWnd);

	bool CreateRenderTarget();

	void CleanupRenderTarget();

	// main rendering loop
	int Render();

	// initializes fonts
	void InitializeFonts();

	//releases directx and clears imgui
	void Shutdown();

	//initializes imgui
	void Renderimgui(HWND hWnd);

	//initializes imgui styles
	void MenuInit();

	//restarts directx drawings
	void ResetDevice();

	void Loops();

	void HeroLoop();

	void TurretLoop();

	void HotkeyButton(int& key, bool mouse = false);
	void HelpMarker(const char* desc);

	ImVec2 GetHpBarPos(CObject& obj)
	{
		Vector3 pos = obj.GetPosition();
		pos.y += obj.GetHpBarHeight();
		ImVec2 w2s = WorldToScreen(pos);
		w2s.y -= (M.nRendererHeight * 0.00083333335f * obj.GetHpBarHeight());

		return w2s;
	}

	//w2s
	ImVec2 WorldToScreen(const Vector3& pos) const;
	ImVec2 WorldToMinimap(const Vector3& pos) const;
};
extern Direct3D9Render Direct3D9;

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, we are using a more C++ like approach of declaring a class to hold both data and functions.
struct ConsoleLog
{
	char                  InputBuf[256];
	ImVector<char*>       Items;
	ImVector<const char*> Commands;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool                  AutoScroll;
	bool                  ScrollToBottom;
	bool                  StopPrinting;
	bool                  IgnoreStopPrint;

	ConsoleLog()
	{
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;

		Commands.push_back("HELP");
		Commands.push_back("HISTORY");
		Commands.push_back("CLEAR");
		Commands.push_back("PAUSE");
		Commands.push_back("RESUME");
		Commands.push_back("EXIT");
		Commands.push_back("REINIT");
		Commands.push_back("LOADCFG");
		Commands.push_back("SAVECFG");

		Commands.push_back("INFO");

		AutoScroll = true;
		ScrollToBottom = false;
		StopPrinting = false;
		IgnoreStopPrint = false;
	}
	~ConsoleLog()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
	static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	static char* Strdup(const char* s) { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

	void    ClearLog()
	{
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		if (!StopPrinting || IgnoreStopPrint)
		{
			Items.push_back(Strdup(buf));
			IgnoreStopPrint = false;
		}
	}

	void    Draw(const char* title, bool* p_open)
	{
		ImGui::SetNextWindowPos(ImVec2(1505, 435), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(389, 605), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
		// So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::SmallButton("Scroll to Bottom")) { ScrollToBottom = true; }
		/*if (ImGui::SmallButton("Add Debug Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); }
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }*/
		ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) { ClearLog(); }
		ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy");

		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw("Filter", 180);
		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets.
		// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
		// to only process visible items. The clipper will automatically measure the height of your first item and then
		// "seek" to display only items in the visible area.
		// To use the clipper we can replace your standard loop:
		//      for (int i = 0; i < Items.Size; i++)
		//   With:
		//      ImGuiListClipper clipper;
		//      clipper.Begin(Items.Size);
		//      while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// - That your items are evenly spaced (same height)
		// - That you have cheap random access to your elements (you can access them given their index,
		//   without processing all the ones before)
		// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
		// We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
		// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
		// to improve this example code!
		// If your items are of variable height:
		// - Split them into same height items would be simpler and facilitate random-seeking into your list.
		// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (int i = 0; i < Items.Size; i++)
		{
			const char* item = Items[i];
			if (!Filter.PassFilter(item))
				continue;

			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool has_color = false;
			if (strstr(item, XorStr("[error]"))) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
			else if (strncmp(item, XorStr("# "), 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
			else if (strstr(item, XorStr("[start]"))) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
			else if (strstr(item, XorStr("[info]"))) { color = ImVec4(0.4f, 1.f, 0.4f, 1.0f); has_color = true; }
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(item);
			if (has_color)
				ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();

		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
		{
			char* s = InputBuf;
			Strtrim(s);
			if (s[0])
				ExecCommand(s);
			strcpy(s, "");
			reclaim_focus = true;
		}

		// Auto-focus on window apparition
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}

	void    ExecCommand(const char* command_line)
	{
		IgnoreStopPrint = true;
		AddLog("# %s\n", command_line);

		// Insert into history. First find match and delete it so it can be pushed to the back.
		// This isn't trying to be smart or optimal.
		HistoryPos = -1;
		for (int i = History.Size - 1; i >= 0; i--)
			if (Stricmp(History[i], command_line) == 0)
			{
				free(History[i]);
				History.erase(History.begin() + i);
				break;
			}
		History.push_back(Strdup(command_line));

		IgnoreStopPrint = true;
		// Process command
		if (Stricmp(command_line, "CLEAR") == 0 || Stricmp(command_line, "CLS") == 0)
		{
			ClearLog();
		}
		else if (Stricmp(command_line, "HELP") == 0)
		{
			AddLog("Commands:");
			for (int i = 0; i < Commands.Size; i++)
			{
				IgnoreStopPrint = true;
				AddLog("- %s", Commands[i]);
			}
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = History.Size - 10;
			for (int i = first > 0 ? first : 0; i < History.Size; i++)
			{
				IgnoreStopPrint = true;
				AddLog("%3d: %s\n", i, History[i]);
			}
		}
		else if (Stricmp(command_line, "PAUSE") == 0)
		{
			AddLog("Paused Logging");
			StopPrinting = true;
		}
		else if (Stricmp(command_line, "RESUME") == 0)
		{
			AddLog("Resumed Logging");
			StopPrinting = false;
		}
		else if (Stricmp(command_line, "EXIT") == 0)
		{
			M.bExitBot = true;
		}
		else if (Stricmp(command_line, "REINIT") == 0)
		{
			IgnoreStopPrint = true;
			AddLog("Reinitializing objects");
			if (!init->herolist.empty()) init->herolist.clear();
			if (!init->turretlist.empty()) init->turretlist.clear();
			if (!init->inhiblist.empty()) init->inhiblist.clear();
			init->Start();
		}
		else if (Stricmp(command_line, "SAVECFG") == 0)
		{
			IgnoreStopPrint = true;
			AddLog("Saved config");
			Config->Save("default");
		}
		else if (Stricmp(command_line, "LOADCFG") == 0)
		{
			IgnoreStopPrint = true;
			AddLog("Loaded config");
			Config->Load("default");
		}
		else if (Stricmp(command_line, "INFO") == 0)
		{
			IgnoreStopPrint = true;
			if (M.bDebug)
				init->StartupInfo();
			else
				AddLog(XorStr("[info] %s"), M.sServerInfo.c_str());
		}
		else
		{
			IgnoreStopPrint = true;
			AddLog("Unknown command: '%s'\n", command_line);
		}

		// On command input, we scroll to bottom even if AutoScroll==false
		ScrollToBottom = true;
	}

	// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		ConsoleLog* console = (ConsoleLog*)data->UserData;
		return console->TextEditCallback(data);
	}

	int     TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		IgnoreStopPrint = true;
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char*> candidates;
			for (int i = 0; i < Commands.Size; i++)
				if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(Commands[i]);

			if (candidates.Size == 0)
			{
				// No match
				AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can..
				// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
				int match_len = (int)(word_end - word_start);
				for (;;)
				{
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}
				IgnoreStopPrint = true;
				// List matches
				AddLog("Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
				{
					IgnoreStopPrint = true;
					AddLog("- %s\n", candidates[i]);
				}
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = HistoryPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (HistoryPos == -1)
					HistoryPos = History.Size - 1;
				else if (HistoryPos > 0)
					HistoryPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (HistoryPos != -1)
					if (++HistoryPos >= History.Size)
						HistoryPos = -1;
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != HistoryPos)
			{
				const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}
};
extern ConsoleLog clog;

#endif //_DIRECTX_H_
