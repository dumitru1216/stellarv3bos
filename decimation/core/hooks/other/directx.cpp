#include "../../../includes.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include "../../../new_gui/m_elements.h"

HRESULT WINAPI Hooks::Present( IDirect3DDevice9* pDevice, RECT* pRect1, const RECT* pRect2, HWND hWnd, const RGNDATA* pRGNData ) {

	static bool initialised_renderer = false;
	if( !initialised_renderer ) {

		Menu::get().Init(pDevice);
		ImGui_ImplWin32_Init(FindWindowA("Valve001", NULL));

		D3D::Initialise( pDevice );
		initialised_renderer = true;
	}
	else {
		g_input.update( );

		InputHelper::Update( );

		bool old_night = g_loser.player_esp.world[0];
		bool old_tickbase = g_tickbase.m_shift_data.m_should_attempt_shift;
		float old_night_darkness = g_loser.player_esp.world_night_darkness;
		float old_prop_opacity = g_loser.player_esp.world_prop_opacity;
		
		bool old_skins = g_cfg[ XOR( "skins_enable" ) ].get<bool>( );
		int old_knife = g_cfg[ XOR( "skins_knife" ) ].get<int>( );
		int old_gloves = g_cfg[ XOR( "skins_gloves" ) ].get<int>( );
		int old_glove_skins = g_cfg[ XOR( "skins_glove_kit" ) ].get<int>( );
		float old_volume = g_loser.miscellaneous.hitsound_volume;
		int old_sound = g_loser.miscellaneous.custom_hitsound;


		D3D::Draw( );
		
		/* new init */
		DrawList.Init( pDevice );
		DrawList.SetupRenderStates( );

		g_cl.DrawHUD( );

		/* new menu */
		Gui::m_Details.Install( );

		if( old_night != g_loser.player_esp.world[0] ||
			old_night_darkness != g_loser.player_esp.world_night_darkness ||
			old_prop_opacity != g_loser.player_esp.world_prop_opacity) {
			g_cl.m_update_night = true;
		}

		if( old_skins != g_cfg[ XOR( "skins_enable" ) ].get<bool>( ) ||
			old_knife != g_cfg[ XOR( "skins_knife" ) ].get<int>( ) ||
			old_gloves != g_cfg[ XOR( "skins_gloves" ) ].get<int>( ) ||
			old_glove_skins != g_cfg[ XOR( "skins_glove_kit" ) ].get<int>( ) ) {
			g_skins.m_update = true;
		}

		if (old_volume != g_loser.miscellaneous.hitsound_volume || old_sound != g_loser.miscellaneous.custom_hitsound) {
			g_cl.m_update_sound = true;
		}

		D3D::End( );

		g_visuals.m_thirdperson = g_config.get_hotkey( XOR( "misc_thirdperson_key" ) );
		g_hvh.m_fake_duck = g_config.get_hotkey( XOR( "rage_aa_fakeduck_key" ) );
		g_movement.m_slow_motion = g_config.get_hotkey( XOR( "movement_fakewalk_key" ) );
		g_aimbot.m_force_safepoint = g_config.get_hotkey(XOR("rage_safepoint_key"));


		g_cl.m_goal_shift = g_loser.rage.rage_exploit_type == 1 ? 15 : 10;

		if ( old_tickbase != g_tickbase.m_shift_data.m_should_attempt_shift ) {
			if ( g_tickbase.m_shift_data.m_should_attempt_shift )
				g_tickbase.m_shift_data.m_needs_recharge = g_cl.m_goal_shift;
			else
				g_tickbase.m_shift_data.m_needs_recharge = 0;

			g_tickbase.m_shift_data.m_did_shift_before = false;
		}

		g_input.updateMouse( );
		g_input.m_scroll = 0;
	}

	return g_hooks.m_device.GetOldMethod<decltype( &Present )>( 17 )( pDevice, pRect1, pRect2, hWnd, pRGNData );
}

HRESULT WINAPI Hooks::Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParameters )
{
	D3D::HandleReset( );

	auto result = g_hooks.m_device.GetOldMethod<decltype( &Reset )>( 16 )( pDevice, pPresentParameters );

	if( result == D3D_OK ) {
		D3D::HandleReset( );
	}

	return result;
}