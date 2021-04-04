#include "Elements.h"
#include "../BACK.h"
#include "../3r.h"
#include "../SKEETIDB.h"
#include <shellapi.h>

void Menu::Init(IDirect3DDevice9* pDevice)
{
	if (m_bInitialized)
		return;

	ImGui::CreateContext();
	auto io = ImGui::GetIO();
	auto style = &ImGui::GetStyle();

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 6.f;
	style->WindowPadding = ImVec2(0, 0);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.2f, 0.2f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

	m_bIsOpen = true;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters(&param);
	pDevice->GetSwapChain(0, &pChain);

	if (pChain)
		pChain->GetPresentParameters(&pp);

	ImGui_ImplWin32_Init(param.hFocusWindow);
	ImGui_ImplDX9_Init(pDevice);

	if (!D3D::menuBg)
		D3DXCreateTextureFromFileInMemoryEx(pDevice, &MenuBG, sizeof(MenuBG),
			648, 548, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, NULL, NULL, &D3D::menuBg);

	ImFontConfig _cfg;
	D3D::tabFont = io.Fonts->AddFontFromMemoryTTF(&tabFont, sizeof tabFont, 50.f, &_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	D3D::boldMenuFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(boldMenuFont, sizeof boldMenuFont, 11, &_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	D3D::menuFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(menuFont, sizeof menuFont, 11, &_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	D3D::controlFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(controlFont, sizeof controlFont, 12, &_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	D3D::weaponFont = io.Fonts->AddFontFromMemoryTTF(&weaponFont, sizeof weaponFont, 26.f, &_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	D3D::keybindsFont = io.Fonts->AddFontFromMemoryTTF(keybindsFont, sizeof keybindsFont, 10.f, &_cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontDefault();

	m_bInitialized = true;
}

void Menu::Begin()
{
	if (!m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Menu::End() {
	if (!m_bInitialized)
		return;

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

inline bool ColorPicker(const char* name, Color* color, bool show_alpha = true) {

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStyle* style = &ImGui::GetStyle();

	float clr[4] = {
			color->r() / 255.0f,
			color->g() / 255.0f,
			color->b() / 255.0f,
			color->a() / 255.0f
	};

	ImGui::SameLine(window->Size.x - 45);
	if (ImGui::ColorEdit4(std::string{ "##" }.append(name).append("Picker").c_str(), clr, show_alpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar))
	{
		color->SetColor(clr[0], clr[1], clr[2], clr[3]);
		return true;
	}
	return false;
}

void Menu::Render() {

	ImGuiStyle* style = &ImGui::GetStyle();

	style->Colors[ImGuiCol_MenuTheme] = ImVec4(0.5882352941176471f, 0.7843137254901961f, 0.2352941176470588f, 1.f);
	style->WindowPadding = ImVec2(6, 6);

	ImGui::PushFont(D3D::menuFont);

	ImGui::SetNextWindowSize(ImVec2(660.f, 560.f));
	ImGui::BeginMenuBackground("скит айдиби нахуй, понял чудище?", &Menu::get().m_bIsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar); {

		ImGui::BeginChild("Complete Border", ImVec2(648.f, 548.f), false); {

			ImGui::Image(D3D::menuBg, ImVec2(648.f, 548.f));

		} ImGui::EndChild();

		ImGui::SameLine(6.f);

		style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);

		ImGui::BeginChild("Menu Contents", ImVec2(648.f, 548.f), false); {

			ImGui::ColorBar("unicorn", ImVec2(648.f, 2.f));

			ImGui::BeginTabs("Tabs", ImVec2(75.f, 542.f), false); {

				style->ItemSpacing = ImVec2(0.f, 0.f);

				style->ButtonTextAlign = ImVec2(0.5f, 0.47f);

				ImGui::PopFont();
				ImGui::PushFont(D3D::tabFont);

				switch (m_nActiveTab) {

				case 0:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::SelectedTab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 1:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::SelectedTab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 2:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::SelectedTab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 3:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::SelectedTab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 4:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::SelectedTab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 5:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::SelectedTab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::Tab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 6:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("A", ImVec2(75.f, 75.f))) m_nActiveTab = 0;
					if (ImGui::Tab("G", ImVec2(75.f, 75.f))) m_nActiveTab = 1;
					if (ImGui::Tab("B", ImVec2(75.f, 75.f))) m_nActiveTab = 2;
					if (ImGui::Tab("C", ImVec2(75.f, 75.f))) m_nActiveTab = 3;
					if (ImGui::Tab("D", ImVec2(75.f, 75.f))) m_nActiveTab = 4;
					if (ImGui::Tab("E", ImVec2(75.f, 75.f))) m_nActiveTab = 5;
					if (ImGui::SelectedTab("F", ImVec2(75.f, 75.f))) m_nActiveTab = 6;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				}

				ImGui::PopFont();
				ImGui::PushFont(D3D::menuFont);

				style->ButtonTextAlign = ImVec2(0.5f, 0.5f);

			} ImGui::EndTabs();

			ImGui::SameLine(75.f);

			ImGui::BeginChild("Tab Contents", ImVec2(572.f, 542.f), false); {

				style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);

				switch (m_nActiveTab) {

				case 0:
					Ragebot();
					break;
				case 1:
					Anti_aimbot();
					break;
				case 2:
					Legitbot();
					break;
				case 3:
					Visuals();
					break;
				case 4:
					Misc();
					break;
				case 5:
					Skins();
					break;
				case 6:
					Players();
					break;
				}

				style->Colors[ImGuiCol_Border] = ImColor(10, 10, 10, 255);

			} ImGui::EndChild();

			style->ItemSpacing = ImVec2(4.f, 4.f);
			style->Colors[ImGuiCol_ChildBg] = ImColor(17, 17, 17, 255);

		} ImGui::EndChild();

		ImGui::PopFont();

	} ImGui::End();
}


void Menu::Ragebot() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Aimbot", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCheckbox(XOR("Scale damage on health"), g_loser.rage.rage_aimbot_minimal_damage_hp);
			if (g_loser.rage.rage_aimbot_penetration) {
				InsertCheckbox(XOR("Scale penetration damage on health"), g_loser.rage.rage_aimbot_penetration_minimal_damage_hp);
			}
			ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);
			ImGui::Text(XOR("Min damage override"));
			InsertKeyBindStyle(XOR("Min damage override key"), g_loser.rage.rage_aimbot_minimal_damage_override_key, g_loser.rage.rage_aimbot_minimal_damage_override_key_style);
			InsertSlider(XOR("Damage override amount"), g_loser.rage.rage_aimbot_minimal_damage_override, 1.f, 100.f, XOR("%.0f"));
			InsertCheckbox(XOR("Override penetration damage"), g_loser.rage.rage_aimbot_minimal_damage_override_pen);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Other", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);
			ImGui::Text(XOR("Force body-aim key"));
			InsertKeyBindStyle(XOR("Force body-aim"), g_loser.rage.rage_aimbot_baim_key, g_loser.rage.rage_aimbot_baim_key_style)

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Other Cover", "Other");
	}
}

void Menu::Anti_aimbot() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Anti-aimbot angles", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCheckbox(XOR("Enable"), g_loser.rage.rage_aa_enabled);



			std::vector<const char*> Direction =
			{
				XOR("Backwards"), XOR("Left"), XOR("Right")
			};

			std::vector<const char*> Jitter_type =
			{
				XOR("None"), XOR("Offset"), XOR("Random")
			};


			auto yaw = g_loser.rage.rage_aa_yaw;
			// yaw types.
			if (yaw > 0) {
				if (yaw == 1)
					InsertComboWithoutText(XOR("Direction"), g_loser.rage.rage_aa_yaw_direction, Direction);

				// random.
				if (yaw == 2) {
					InsertSlider(XOR("Random update"), g_loser.rage.rage_aa_random_update, 0.f, 1.f, XOR("%.1fs"));
				}

				InsertComboWithoutText(XOR("Jitter type"), g_loser.rage.rage_aa_jitter_type, Jitter_type);
				InsertSlider(XOR("Jitter range"), g_loser.rage.rage_aa_jitter_range, 0.f, 361.f, XOR("%.0f"));

				InsertCheckbox(XOR("Rotation"), g_loser.rage.rage_aa_rotation_enabled);

				if (g_loser.rage.rage_aa_rotation_enabled) {
					InsertSlider(XOR("Rotate range"), g_loser.rage.rage_aa_rotation_range, 1.f, 361.f, XOR("%.0f"));
					InsertSlider(XOR("Rotate speed"), g_loser.rage.rage_aa_rotation_speed, 0.f, 100.f, XOR("%.0f"));
				}
			}

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Anti-aimbot angles Cover", "Anti-aimbot angles");
	}
	ImGui::NextColumn(); {
		InsertGroupBoxRight("Fake lag", 331.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);



			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Fake lag Cover", "Fake lag");

		InsertSpacer("Fake lag - Other Spacer");

		InsertGroupBoxRight("Fake", 157.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			std::vector<const char*> real_around_fake =
			{
				XOR("Standing"), XOR("Moving"), XOR("In-Air"), XOR("Slow motion")
			};

			std::vector<const char*> dddd =
			{
				XOR("Off"), XOR("Static")
			};

			InsertComboWithoutText(XOR("Yaw"), g_loser.rage.rage_aa_yaw_fake, dddd);
			InsertMultiCombo(XOR("Real around fake conditions"), g_loser.rage.rage_aa_real_around_fake, real_around_fake, 4);
			InsertCheckbox(XOR("Real around fake jitter"), g_loser.rage.rage_aa_real_around_fake_jitter);

			InsertSlider(XOR("Fake amount"), g_loser.rage.rage_aa_custom_fake, -60.f, 60.f, XOR("%.0f%%"));

			ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);
			ImGui::Text(XOR("Invert"));
			InsertKeyBindStyle(XOR("Negate key"), g_loser.rage.rage_aa_negate_key, g_loser.rage.rage_aa_negate_key_style);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Fake Cover", "Fake");
	}
}

void Menu::Legitbot() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	InsertGroupBoxTop("Weapon Selection", ImVec2(535.f, 61.f)); {
		ImGui::Columns(6, nullptr, false);
		ImGui::PopFont();
		ImGui::PushFont(D3D::weaponFont);
		style->ButtonTextAlign = ImVec2(0.5f, 0.75f);
		{
			if (ImGui::TabButton("G", ImVec2(80, 45)))
				m_nActiveLegitTab = 0;

			ImGui::NextColumn();
			{
				if (ImGui::TabButton("P", ImVec2(80, 45)))
					m_nActiveLegitTab = 1;
			}
			ImGui::NextColumn();
			{
				if (ImGui::TabButton("W", ImVec2(80, 45)))
					m_nActiveLegitTab = 2;
			}
			ImGui::NextColumn();
			{
				if (ImGui::TabButton("d", ImVec2(80, 45)))
					m_nActiveLegitTab = 3;
			}
			ImGui::NextColumn();
			{
				if (ImGui::TabButton("f", ImVec2(80, 45)))
					m_nActiveLegitTab = 4;
			}
			ImGui::NextColumn();
			{
				if (ImGui::TabButton("a", ImVec2(80, 45)))
					m_nActiveLegitTab = 5;
			}
		}
	} InsertEndGroupBoxTop("Weapon Selection Cover", "Weapon Selection", ImVec2(536.f, 11.f));

	InsertSpacer("Weapon Selection - Main Group boxes Spacer");

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Aimbot", 427.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);



			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Triggerbot", 277.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);



			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");

		InsertSpacer("Triggerbot - Other Spacer");

		InsertGroupBoxRight("Other", 132.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);



			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Other Cover", "Other");
	}
}

std::string multiselect_comboboxes_previews[2];

void Menu::Visuals() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Player ESP", 331.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			
			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Player ESP Cover", "Player ESP");

		InsertSpacer("Player ESP - Colored models Spacer");

		InsertGroupBoxLeft("Colored models", 157.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);


			

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Colored models Cover", "Colored models");
	}
	ImGui::NextColumn(); {
		InsertGroupBoxRight("Other ESP", 199.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			std::vector<const char*> Bomb = {
				XOR("Dropped"),
				XOR("On screen (2D)"),
				XOR("On bomb (3D)")
			};

			InsertCheckbox(XOR("Radar"), g_loser.player_esp.enemies_radar);

			InsertCheckbox(XOR("Force crosshair"), g_loser.player_esp.force_crosshair);
			InsertCheckbox(XOR("Penetration crosshair"), g_loser.player_esp.penetration_crosshair);

			InsertCheckbox(XOR("Grenades"), g_loser.player_esp.projectiles);
			ColorPicker(XOR("Grenades color"), &g_loser.player_esp.projectiles_color);

			InsertMultiCombo(XOR("Bomb ESP"), g_loser.player_esp.bomb, Bomb, 3);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Other ESP Cover", "Other ESP");

		InsertSpacer("Other ESP - Effects Spacer");

		InsertGroupBoxRight("Effects", 289.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);


			InsertCheckbox(XOR("Remove teammate rendering"), g_loser.player_esp.no_draw_team);

			std::vector<const char*> beam = {

			  XOR("Beam"), XOR("Line")

			};

			InsertSlider(XOR("Prop opacity"), g_loser.player_esp.world_prop_opacity, 0.f, 100.f, XOR("%.0f%%"));

			InsertCheckbox(XOR("Bullet tracers"), g_loser.player_esp.bullet_beam);

			if (g_loser.player_esp.bullet_beam) {
				InsertComboWithoutText(XOR("Bullet tracers type"), g_loser.player_esp.bullet_beam_type, beam);

				InsertSlider(XOR("Bullet tracers duration"), g_loser.player_esp.bullet_beam_time, 1.f, 15.f, XOR("%.0fs"));

				ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);
				ImGui::Text(XOR("Bullet tracers color"));
				ColorPicker(XOR("Bullet tracers colorr"), &g_loser.player_esp.bullet_beam_color);
				ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);
				ImGui::Text(XOR("Bullet tracers hurt color"));
				ColorPicker(XOR("Bullet tracers hurt colorr"), &g_loser.player_esp.bullet_beam_hurt_color);
			}


			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Effects Cover", "Effects");
	}
}

void Menu::Misc() {
	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Miscellaneous", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);


			ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f);


			InsertCheckbox(XOR("Override viewmodel field-of-view"), g_loser.miscellaneous.override_viewmodel_fov);
			if (g_loser.miscellaneous.override_viewmodel_fov) {
				InsertSlider(XOR("Viewmodel field-of-view"), g_loser.miscellaneous.override_viewmodel_fov_amount, 60.f, 100.f, XOR("%.0f"));
			}


			std::vector<const char*> log1 = {
			 XOR("Damage given"),
			 XOR("Damage taken"),
			 XOR("Purchases"),
			 XOR("Bomb"), 
			 XOR("Defuse")
			};

			InsertMultiCombo(XOR("Notifications"), g_loser.miscellaneous.log, log1, 5);

			InsertCheckbox(XOR("Clantag spammer"), g_loser.miscellaneous.clantag);
			std::vector<const char*> xui2 =
			{
				XOR("Off"), XOR("Screen")
			};
			std::vector<const char*> xui3 =
			{
				XOR("Off"), XOR("Arena switch press")
			};
			InsertComboWithoutText(XOR("Hitmarker"), g_loser.miscellaneous.hitmarker, xui2);
			if (g_loser.miscellaneous.hitmarker != 0)
			{
				InsertComboWithoutText(XOR("Hitsound"), g_loser.miscellaneous.hitsound, xui3);
				InsertSlider(XOR("Hitsound volume"), g_loser.miscellaneous.hitsound_volume, 1.f, 100.f, XOR("%.0f%%"));
			}
			InsertCheckbox(XOR("Ragdoll force"), g_loser.miscellaneous.ragdoll_force);
			InsertCheckbox(XOR("Unlock inventory in-game"), g_loser.miscellaneous.unlock_inventory);


			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Miscellaneous Cover", "Miscellaneous");
	}
	ImGui::NextColumn(); {
		InsertGroupBoxRight("Settings", 156.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Settings Cover", "Settings");

		InsertSpacer("Settings - Other Spacer");

		InsertGroupBoxRight("Other", 332.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			style->ButtonTextAlign = ImVec2(0.5f, -2.0f);
			ImGui::CustomSpacing(9.f);

			static int current_preset;

			std::vector<const char*> presets = {
				XOR("Legit"),
				XOR("Rage"),
				XOR("HvH"),
				XOR("Secret"),
				XOR("Headshot"),
				XOR("Alpha"),
				XOR("Bravo"),
				XOR("Charlie"),
				XOR("Delta"),
				XOR("Echo"),
				XOR("Foxtrot"),
				XOR("Golf"),
				XOR("Hotel"),
				XOR("India"),
				XOR("Juliet"),
				XOR("Kilo")
			};

			InsertCombo("Presets", current_preset, presets);

			ImGui::CustomSpacing(6.f); ImGui::NewLine(); ImGui::SameLine(42.f);

			// ставим трушную длину для ебаных кнопок.
			ImGui::PushItemWidth(158.f);

			// логи для кфг по приколу добавил, хз нахуй они нужны но пусть будут.
			if (ImGui::Button("Load config"))
			{
				if (g_loser.Config.Load(presets[current_preset]))
				{
					g_notify.add(XOR("Successfully loading cfg\n"));
				}
				else
				{
					g_notify.add(XOR("Error when loading cfg\n"));
				}
			}

			ImGui::CustomSpacing(6.f); ImGui::NewLine(); ImGui::SameLine(42.f);
			if (ImGui::Button("Save config"))
			{
				g_loser.Config.CreateConfig(presets[current_preset]);
				if (g_loser.Config.Save(presets[current_preset]))
				{
					g_notify.add(XOR("Successfully saved cfg\n"));
				}
				else
				{
					g_notify.add(XOR("Error when saving cfg\n"));
				}
			}

			ImGui::CustomSpacing(6.f); ImGui::NewLine(); ImGui::SameLine(42.f);
			if (ImGui::Button("Open configs folder"))
				ShellExecute(0, "open", "C:/immortality/Configs", NULL, NULL, SW_NORMAL);
			// возвращаем все как было.
			ImGui::PopItemWidth();

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);
		} InsertEndGroupBoxRight("Other Cover", "Other");
	}
}

void Menu::Skins() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

	}
}

void Menu::Players() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Players", 506.f); {


		} InsertEndGroupBoxLeft("Players Cover", "Players");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Adjustments", 506.f); {


		} InsertEndGroupBoxRight("Adjustments Cover", "Adjustments");
	}
}