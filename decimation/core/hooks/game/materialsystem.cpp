#include "../../../includes.h"

bool Hooks::OverrideConfig( MaterialSystem_Config_t* config, bool update ) {
	if(g_loser.player_esp.world[1])
		config->m_nFullbright = true;

	if (g_cl.m_update_night) {
		g_visuals.ModulateWorld();

		g_cl.m_update_night = false;
	}

	return g_hooks.m_material_system.GetOldMethod< OverrideConfig_t >( IMaterialSystem::OVERRIDECONFIG )( this, config, update );
}