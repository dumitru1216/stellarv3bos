#include "../includes.h"

#define dont

TickbaseSystem g_tickbase;

void TickbaseSystem::WriteUserCmd(bf_write* buf, CUserCmd* incmd, CUserCmd* outcmd) {
	// Generic WriteUserCmd code as seen in every hack ever
	using WriteUsercmd_t = void(__fastcall*)(void*, CUserCmd*, CUserCmd*);
	static WriteUsercmd_t fnWriteUserCmd = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D")).as<WriteUsercmd_t>();

	__asm {
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    fnWriteUserCmd
		add     esp, 4
	}
}

bool Hooks::WriteUsercmdDeltaToBuffer(int m_nSlot, void* m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd) {
	if ( g_tickbase.m_shift_data.m_ticks_to_shift <= 0 || g_csgo.m_cl->m_choked_commands > 3 )
		return g_hooks.m_client.GetOldMethod<WriteUsercmdDeltaToBuffer_t>( CHLClient::USRCMDTODELTABUFFER )( this, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );

	if ( m_nFrom != -1 )
		return true;
		m_nFrom = -1;

	uintptr_t stackbase;
	__asm mov stackbase, ebp;
	CCLCMsg_Move_t* msg = reinterpret_cast< CCLCMsg_Move_t* >( stackbase + 0xFCC );
	auto net_channel = *reinterpret_cast < INetChannel** > ( reinterpret_cast < uintptr_t > ( g_csgo.m_cl ) + 0x9C );
	int32_t new_commands = msg->new_commands;

	int32_t next_cmdnr = g_csgo.m_cl->m_last_outgoing_command + g_csgo.m_cl->m_choked_commands + 1;

	int32_t total_new_commands = std::min( g_tickbase.m_shift_data.m_ticks_to_shift, 16 );
	g_tickbase.m_shift_data.m_ticks_to_shift -= total_new_commands;

	m_nFrom = -1;
	msg->new_commands = total_new_commands;
	msg->backup_commands = 0;

	for ( m_nTo = next_cmdnr - new_commands + 1; m_nTo <= next_cmdnr; m_nTo++ ) {
		if ( !g_hooks.m_client.GetOldMethod< WriteUsercmdDeltaToBuffer_t >( CHLClient::USRCMDTODELTABUFFER )( this, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd ) )
			return false;

		m_nFrom = m_nTo;
	}

	CUserCmd* last_realCmd = g_csgo.m_input->GetUserCmd( m_nSlot, m_nFrom );
	CUserCmd fromCmd;

	if ( last_realCmd )
		fromCmd = *last_realCmd;

	CUserCmd toCmd = fromCmd;
	toCmd.m_command_number++;
	toCmd.m_tick++;

	for ( int i = new_commands; i <= total_new_commands; i++ ) {
		g_tickbase.WriteUserCmd( ( bf_write* )m_pBuffer, &toCmd, &fromCmd );
		fromCmd = toCmd;
		toCmd.m_command_number++;
		toCmd.m_tick++;
	}
	return true;
}

void TickbaseSystem::PreMovement() {

	// Invalidate next shift amount and the ticks to shift prior to shifting
	g_tickbase.m_shift_data.m_next_shift_amount = g_tickbase.m_shift_data.m_ticks_to_shift = 0;
}

void TickbaseSystem::PostMovement() {
	// Perform sanity checks to make sure we're able to shift
	if (!g_cl.m_processing || g_loser.rage.rage_exploit_type == 0) {
		return;
	}

	if (!g_cl.m_cmd || !g_cl.m_weapon) {
		return;
	}

	if ( g_cl.m_weapon_id == REVOLVER ||
		g_cl.m_weapon_id == C4 ||
		g_cl.m_weapon_type == WEAPONTYPE_KNIFE ||
		g_cl.m_weapon->IsGrenade( ) )
	{
		g_tickbase.m_shift_data.m_did_shift_before = false;
		g_tickbase.m_shift_data.m_should_be_ready = false;
		g_tickbase.m_shift_data.m_should_disable = true;
		return;
	}

	// Don't attempt to shift if we're supposed to
	if ( !g_tickbase.m_shift_data.m_should_attempt_shift ) {
		g_tickbase.m_shift_data.m_did_shift_before = false;
		g_tickbase.m_shift_data.m_should_be_ready = false;
		return;
	}


	g_tickbase.m_shift_data.m_should_disable = false;

	// Setup variables we will later use in order to dermine if we're able to shift tickbase or not

	// This could be made into a checkbox 
	bool bFastRecovery = false;

	float flNonShiftedTime = game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase() - g_cl.m_goal_shift);

	// Determine if we are able to shoot right now (at the time of the shift)
	bool bCanShootNormally = g_cl.CanFireWeapon(game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase()));

	// Determine if we are able to shoot in the previous iShiftAmount ticks (iShiftAmount ticks before we shifted)
	bool bCanShootIn12Ticks = g_cl.CanFireWeapon(flNonShiftedTime);

	bool bIsShooting = g_cl.IsFiring(game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase()));

	// Determine if we are able to shift the tickbase respective to previously setup variables (rofl)
	g_tickbase.m_shift_data.m_can_shift_tickbase = bCanShootIn12Ticks || (!bCanShootNormally || bFastRecovery) && (g_tickbase.m_shift_data.m_did_shift_before);

	// If we can shift tickbase, shift enough ticks in order to double-tap
	if ( g_tickbase.m_shift_data.m_can_shift_tickbase && !g_hvh.m_fake_duck ) {
		// Tell the cheat to shift tick-base and disable fakelag
		g_tickbase.m_shift_data.m_next_shift_amount = g_cl.m_goal_shift;
	}
	else {
		g_tickbase.m_shift_data.m_next_shift_amount = 0;
		g_tickbase.m_shift_data.m_should_be_ready = false;
	}

	// we want to recharge after stopping fake duck.
	if ( g_hvh.m_fake_duck ) {
		g_tickbase.m_shift_data.m_prepare_recharge = true;

		g_tickbase.m_shift_data.m_next_shift_amount = 0;
		g_tickbase.m_shift_data.m_should_be_ready = false;

		g_tickbase.m_shift_data.m_should_disable = true;

		return;
	}

	if ( g_tickbase.m_shift_data.m_next_shift_amount > 0 ) {
		if ( bCanShootIn12Ticks ) {
			if ( g_tickbase.m_shift_data.m_prepare_recharge && !bIsShooting ) {
				g_tickbase.m_shift_data.m_needs_recharge = g_cl.m_goal_shift;
				g_tickbase.m_shift_data.m_prepare_recharge = false;
			}
			else {
				if ( bIsShooting ) {
					// Store shifted command
					g_tickbase.m_prediction.m_shifted_command = g_cl.m_cmd->m_command_number;

					// Store shifted ticks
					g_tickbase.m_prediction.m_shifted_ticks = abs( g_tickbase.m_shift_data.m_current_shift );

					// Store original tickbase
					g_tickbase.m_prediction.m_original_tickbase = g_cl.m_local->m_nTickBase( );

					// Update our wish ticks to shift, and later shift tickbase
					g_tickbase.m_shift_data.m_ticks_to_shift = g_tickbase.m_shift_data.m_next_shift_amount;
				}
			}
		}
		else {
			g_tickbase.m_shift_data.m_prepare_recharge = true;
			g_tickbase.m_shift_data.m_should_be_ready = false;
		}
	}
	else {
		g_tickbase.m_shift_data.m_prepare_recharge = true;
		g_tickbase.m_shift_data.m_should_be_ready = false;
	}

	// Note DidShiftBefore state 
	g_tickbase.m_shift_data.m_did_shift_before = g_tickbase.m_shift_data.m_next_shift_amount > 0;
}