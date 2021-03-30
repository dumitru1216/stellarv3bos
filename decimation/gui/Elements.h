#pragma once
#include "../sing.h"
#include <filesystem>
#include "Framework/GUI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../include/imgui/imgui_impl_dx9.h"
#include "../include/imgui/imgui_impl_win32.h"

class Menu : public singleton<Menu> {

	public:
		void Render();
		void Init(IDirect3DDevice9 * pDevice);
		void Begin();
		void End();

		void Ragebot();
		void Anti_aimbot();
		void Legitbot();
		void Visuals();
		void Misc();
		void Skins();
		void Players();
		int m_nActiveTab = 4;
		int m_nActiveLegitTab = 0;
		bool m_bInitialized = false;
		bool m_bIsOpen = false;
};