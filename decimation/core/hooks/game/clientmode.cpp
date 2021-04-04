#include "../../../includes.h"
#include "../../../new_gui/m_elements.h"
bool Hooks::ShouldDrawParticles( ) {
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawParticles_t >( IClientMode::SHOULDDRAWPARTICLES )( this );
}

bool Hooks::ShouldDrawFog( ) {
	// remove fog.
	if(g_loser.player_esp.remove_fog)
		return false;

	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawFog_t >( IClientMode::SHOULDDRAWFOG )( this );
}

void Hooks::OverrideView( CViewSetup* view ) {
	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	//g_grenades.think( );
	g_visuals.ThirdpersonThink( );

	if( g_cl.m_processing ) {
		if(g_hvh.test) {
			view->m_origin.z = g_cl.m_local->GetAbsOrigin( ).z + 64.f;
		}
	}

	// call original.
	g_hooks.m_client_mode.GetOldMethod< OverrideView_t >( IClientMode::OVERRIDEVIEW )( this, view );

	// remove scope edge blur.
	if( g_cl.m_local ) {
		//view->m_edge_blur = 0;
		game::SetPostProcess( false );
	}
}

bool Hooks::CreateMove( float time, CUserCmd* cmd ) {
	Stack   stack;
	bool    ret;

	// let original run first.
	ret = g_hooks.m_client_mode.GetOldMethod< CreateMove_t >( IClientMode::CREATEMOVE )( this, time, cmd );


	// called from CInput::ExtraMouseSample -> return original.
	if( !cmd || !cmd->m_command_number )
		return ret;

	// if we arrived here, called from -> CInput::CreateMove
	// call EngineClient::SetViewAngles according to what the original returns.
	if( ret )
		g_csgo.m_engine->SetViewAngles( cmd->m_view_angles );

	// random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
	cmd->m_random_seed = g_csgo.MD5_PseudoRandom( cmd->m_command_number ) & 0x7fffffff;

	// get bSendPacket off the stack.
	g_cl.m_packet = stack.next( ).local( 0x1c ).as< bool* >( );
	*g_cl.m_packet = true;

	// get bFinalTick off the stack.
	g_cl.m_final_packet = stack.next( ).local( 0x1b ).as< bool* >( );

	// remove in_attack flags while the gui is open to maintain ability of moving/walking but not shooting while changing options in the gui.
	if( Gui::m_Details.GetMenuState( ) ) {
		// are we IN_ATTACK?
		if( cmd->m_buttons & IN_ATTACK ) {
			// remove the flag :D!
			cmd->m_buttons &= ~IN_ATTACK;
		}

		// are we IN_ATTACK2?
		if( cmd->m_buttons & IN_ATTACK2 ) {
			// remove the flag :D!
			cmd->m_buttons &= ~IN_ATTACK2;
		}
	}

	// let's better be setting unpredicted curtime when dead.. (fixes clantag and other stuff in the future)
	if( g_cl.m_local && !g_cl.m_local->alive( ) )
		g_inputpred.m_stored_variables.m_flCurtime = g_csgo.m_globals->m_curtime;

	// invoke move function.
	g_cl.OnTick( cmd );

	// make sure to update our animations with the right angles.
	if( cmd->m_command_number >= g_cl.m_shot_command_number && g_cl.m_shot_command_number >= cmd->m_command_number - g_csgo.m_cl->m_choked_commands ) {
		g_cl.m_angle = g_csgo.m_input->m_commands[ g_cl.m_shot_command_number % 150 ].m_view_angles;
	}
	else {
		g_cl.m_angle = cmd->m_view_angles;
	}

	// make sure to update our animations at the right time.
	g_cl.m_animate = true;
	g_cl.m_update_fake = true;

	return false;
}

bool Hooks::DoPostScreenSpaceEffects( CViewSetup* setup ) {
	g_visuals.RenderGlow( );

	g_cl.m_in_glow = true;
	auto result = g_hooks.m_client_mode.GetOldMethod< DoPostScreenSpaceEffects_t >( IClientMode::DOPOSTSPACESCREENEFFECTS )( this, setup );
	g_cl.m_in_glow = false;

	return result;
}