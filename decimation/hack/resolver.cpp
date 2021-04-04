#include "../includes.h"

Resolver g_resolver{ };;

// save these for later
float GetBackwardYaw(Player* player) {
	if (!g_cl.m_processing)
		return -1.f;

	return math::CalcAngle(g_cl.m_local->m_vecOrigin(), player->m_vecOrigin()).y;
}

float GetAngle(Player* player) {
	return math::NormalizedAngle(player->m_angEyeAngles().y);
}

float GetForwardYaw(Player* player) {
	return math::NormalizedAngle(GetBackwardYaw(player) - 180.f);
}

bool Resolver::has_fake(Player* entity, LagComp::LagRecord_t* record)
{
	float simtime = entity->m_flSimulationTime();
	float oldsimtime = entity->m_flOldSimulationTime();
	float simdiff = simtime - oldsimtime;

	auto chokedpackets = game::TIME_TO_TICKS(std::max(0.f, simdiff)); // you could also include latency

	// they are likely not desyncing that much if they are in air
	if (!(entity->m_fFlags() & FL_ONGROUND))
		return false;

	if (fabs(entity->m_flSimulationTime() - entity->m_flOldSimulationTime()) == g_csgo.m_globals->m_interval)
		return false;

	if (chokedpackets < 1) // player not desyncing
		return false;

	return true;
}

int last_resolving_side[65];
void Resolver::ResolveAngles(Player* player, LagComp::LagRecord_t* record) {
	AimPlayer* data = &g_aimbot.m_players[player->index() - 1];

	auto& pdata = inform[player->index() - 1];

	CCSGOPlayerAnimState animstate_backup;

	animstate_backup = *player->m_PlayerAnimState();

	if (g_resolver.has_fake(player, record) == false)
		return;

	// don't resolve shooting players
	if (record->m_bDidShot)
		return;

	//store_delta(player, &pdata, record);

	*player->m_PlayerAnimState() = animstate_backup;

	if (!record->m_bDidShot) {
		if (g_cfg[XOR("cheat_mode")].get<int>() == 1) {
			switch (data->m_missed_shots % 6) {
			case 0:
			case 1:
				record->m_angEyeAngles.x = player->m_angEyeAngles().x = 90.f;
				break;

			case 2:
			case 3:
				record->m_angEyeAngles.x = player->m_angEyeAngles().x = 0.f;
				break;
			case 4:
			case 5:
				record->m_angEyeAngles.x = player->m_angEyeAngles().x = -90.f;
				break;

			default:
				break;
			}
		}

		if (!game::IsFakePlayer(record->m_iEntIndex)) // fix resolving botz
			//ResolveStand(data, record);
		{
			auto speed = player->m_vecVelocity().length_2d();
			if (speed < 0.1f)
			{
				auto layers = &player->m_AnimOverlay()[6];
				if (layers->m_cycle > 0.9f && layers->m_weight > 0.9f)
				{
					auto lby_delta = player->m_flLowerBodyYawTarget() - player->m_angEyeAngles().y;
					auto absed_lby_delta = std::abs(math::NormalizedAngle(lby_delta));
					if (absed_lby_delta > 30.f)
					{
						if (lby_delta > 0.f)
						{
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y + 60.f);
							last_resolving_side[player->index()] = 1;
						}
						else if (lby_delta < 0.f)
						{
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y - 60.f);
							last_resolving_side[player->index()] = -1;
						}
					}
				}
				else
				{
					if (data->m_missed_shots == 0)
						record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y + 60.f * last_resolving_side[player->index()]);
					else
					{
						switch (data->m_missed_shots % 3)
						{
						case 0:
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y - 60.f * last_resolving_side[player->index()]);
							break;
						case 1:
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y + 60.f * last_resolving_side[player->index()]);
							break;
						case 2:
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y);
							break;
						}
					}
				}
			}
			else
			{
				if (data->m_missed_shots == 0)
				{
					float delta1 = abs(record->m_pLayers[3].m_playback_rate - pdata.resolver_anim_layers[0][3].m_playback_rate);
					float delta2 = abs(record->m_pLayers[3].m_playback_rate - pdata.resolver_anim_layers[1][3].m_playback_rate);
					float delta3 = abs(record->m_pLayers[3].m_playback_rate - pdata.resolver_anim_layers[2][3].m_playback_rate);

					if (delta1 < delta3 || delta2 <= delta3 || (int)(delta3 * 1000.f))
					{
						if (delta1 >= delta2 && delta3 > delta2 && !(int)(delta2 * 1000.f))
						{
							last_resolving_side[player->index()] = -1;
							record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y - 60.f);
						}
					}
					else
					{
						last_resolving_side[player->index()] = 1;
						record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y + 60.f);
					}
				}
				else
				{
					switch (data->m_missed_shots % 3)
					{
					case 0:
						record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y - 60.f * last_resolving_side[player->index()]);
						break;
					case 1:
						record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y + 60.f * last_resolving_side[player->index()]);
						break;
					case 2:
						record->m_pState->m_flGoalFeetYaw = math::NormalizedAngle(player->m_angEyeAngles().y);
						break;
					}
				}
			}
		}
	}
}



void Resolver::ResolveStand(AimPlayer* data, LagComp::LagRecord_t* record) {
	// get the players max rotation.
	float max_rotation = record->m_pEntity->GetMaxBodyRotation();

	// setup a starting brute angle.
	float resolve_value = 60.f;

	if (!record->m_pState)
		return;

	const auto info = g_anims.GetAnimationInfo(record->m_pEntity);
	if (!info)
		return;

	float eye_yaw = record->m_pState->m_flEyeYaw;

	// clamp our starting brute angle, to not brute an angle, we most likely can't hit.
	if (max_rotation > resolve_value)
		resolve_value = max_rotation;

	// detect if player is using dick desync
	data->m_extending = record->m_pLayers[6].m_cycle == 0.f && record->m_pLayers[6].m_weight == 0.f;

	if (data->m_extending)
		resolve_value = max_rotation;

	// resolve shooting players separately.
	if (record->m_bDidShot)
		info->m_flBrute = Resolver::ResolveShot(data, record);
	// bruteforce player accordingly.
	else {
		float lbyt = record->m_flLowerBodyYawTarget; //record->m_pEntity->m_flLowerBodyYawTarget( );
	//	data->m_delta = std::abs( math::NormalizedAngle( eye_yaw - lbyt ) );
		data->m_delta = math::NormalizedAngle(eye_yaw - lbyt);

		float resolve_yaw = 0;

		if (fabs(data->m_delta) >= 58)
		{
			resolve_yaw = ((data->m_delta <= 0.f) ? -resolve_value : resolve_value);
		}

		switch (data->m_missed_shots % 3) {
		case 0:
			info->m_flBrute = data->m_last_resolve = resolve_yaw;
			break;
		case 1:
			info->m_flBrute = data->m_last_resolve * (-1);
			break;
		case 2:
			info->m_flBrute = 0;
			break;
		}
	}

	record->m_pState->m_flGoalFeetYaw = std::remainderf(eye_yaw + info->m_flBrute, 360);
}

float Resolver::ResolveShot(AimPlayer* data, LagComp::LagRecord_t* record) {
	float flPseudoFireYaw = math::NormalizedAngle(math::CalcAngle(record->m_pMatrix[8].GetOrigin(), g_cl.m_local->m_BoneCache().m_pCachedBones[0].GetOrigin()).y);

	if (data->m_extending) {
		float flLeftFireYawDelta = fabsf(math::NormalizedAngle(flPseudoFireYaw - (record->m_angEyeAngles.y + 58.f)));
		float flRightFireYawDelta = fabsf(math::NormalizedAngle(flPseudoFireYaw - (record->m_angEyeAngles.y - 58.f)));

		g_notify.add(tfm::format(XOR("found shot record on %s: [ yaw: %i ]"), game::GetPlayerName(record->m_pEntity->index()), int(flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f)));

		return flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f;
	}
	else {
		float flLeftFireYawDelta = fabsf(math::NormalizedAngle(flPseudoFireYaw - (record->m_angEyeAngles.y + 58.f)));
		float flRightFireYawDelta = fabsf(math::NormalizedAngle(flPseudoFireYaw - (record->m_angEyeAngles.y - 58.f)));

		g_notify.add(tfm::format(XOR("found shot record on %s: [ yaw: %i ]"), game::GetPlayerName(record->m_pEntity->index()), int(flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f)));

		return flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f;


	}
}