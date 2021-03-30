#pragma once

class ShotRecord;

struct resolver_recode
{
	float spawn_time;
	bool did_shot_this_tick;
	float resolved_rotation;
	bool breaking_lc;
	int resolving_way;
	int antifreestand_method;

	void Reset()
	{
		/*last_balance_adjust_trigger_time = 0.f;
		just_stopped_delta = 0.f;
		last_time_down_pitch = 0.f;
		last_time_moving = 0.f;
		last_moving_lby = 0.0000001337f;*/

		is_dormant = false;
		/*is_last_moving_lby_valid = false, is_fakewalking = false, is_just_stopped = false, is_breaking_lby = false;*/
		//is_balance_adjust_triggered = false, is_balance_adjust_playing = false, last_moving_lby_can_be_invalid = false;
		//did_lby_flick = false;
		resolved = false;
		fakeangles = false;

		m_flRate = 0.0f;
		m_flServerTorso = 0.0f;

		m_flClientRate = 0.0f;
		m_flLastFlickTime = FLT_MAX;
		m_iSide = -1;
		m_iCurrentSide = -1;
		last_time_balanced = 0.0f;
		last_cycle_desync = 0.0f;
		last_cycle = 0.f;
		b_bRateCheck = false;
		had_fake = false;
		last_time_standed = 0.f;
		last_time_choked = 0.f;
		last_time_three = 0.f;
		last_time_moved = 0.f;
		last_shot_time = 0.f;
		previous_rotation = 0.f;
		//for (auto i = 0; i < 10; i++)
		//	missed_shots[i] = 0;

		current_tick_max_delta = 0.f;
		tick_delta = 0.0f;
		lby_delta = 0.0f;
		abs_yaw_delta = 0.0f;
		latest_delta_used = 0.0f;
		last_speed = 0.0f;
		last_lby = 0.0f;

		is_jittering = false;
		is_using_static = false;
		is_using_balance = false;
		did_shot_this_tick = false;
		did_lby_update_fail = false;
		force_resolving = false;
		is_shifting = false;
		prev_pose = 0.f;
		last_abs_yaw_delta = 0.f;
		last_abs_yaw_delta_change = 0.f;
		prev_delta = FLT_MAX;
		last_abs_yaw_delta_60 = 0.0f;
		/*did_hit_low_delta = false;
		did_hit_max_delta = false;
		did_hit_no_delta = false;*/
		inverse_lby = false;
		did_hit_inversed_lby = false;
		was_shifting = false;
		memset(missed_shots, 0, sizeof(int) * 10);
		//spawntime = 0.0f;
		animations_updated = false;
		preserver_animlayers_saved = false;

		resolver_method_text.clear();

		cfirstmisses = 0;

		previous_side = 0.f;
		cur_side = 0.f;
		breaking_lc = false;
		freestand_fixed = false;


		force_velocity = false;
		new_velocity = vec3_t(0, 0, 0);
		old_velocity = vec3_t(0, 0, 0);
		did_force_velocity = false;

		memset(leftmx, 0, sizeof(matrix3x4_t) * 128);
		memset(rightmx, 0, sizeof(matrix3x4_t) * 128);
		memset(nodesmx, 0, sizeof(matrix3x4_t) * 128);
	}

	struct AntiFreestandingRecord
	{
		int right_damage = 0, left_damage = 0;
		float right_fraction = 0.f, left_fraction = 0.f;

		void reset()
		{
			right_damage = 0;
			left_damage = 0;
			right_fraction = 0.f;
			left_fraction = 0.f;
		}
	};

	vec3_t previous_angles;
	float prev_pose;
	vec3_t last_real_angles;
	vec3_t latest_angles;
	vec3_t latest_fake;
	vec3_t latest_angles_when_faked;
	float previous_rotation;
	float last_time_balanced;
	float last_time_choked;
	float last_time_three;
	float last_cycle_desync;
	float last_cycle;
	bool  had_fake;
	float last_time_standed;
	float last_time_moved;
	float last_abs_yaw_delta;
	float last_abs_yaw_delta_change;
	float last_abs_yaw_delta_60;
	float last_simtime;
	float resolved_yaw;

	float previous_side;
	float cur_side;

	bool force_velocity;
	bool did_force_velocity;
	vec3_t new_velocity;
	vec3_t old_velocity;

	bool freestand_fixed = false;

	float latest_delta_used;

	C_AnimationLayer client_anim_layers[15];

	C_AnimationLayer resolver_anim_layers[3][15];

	C_AnimationLayer server_anim_layers[15];
	C_AnimationLayer preserver_anim_layers[15];
	bool preserver_animlayers_saved = false;
	AntiFreestandingRecord freestanding_record;
	float last_shot_time = 0.0f;
	float spawntime = 0.0f;
	float m_flServerTorso = 0.0f;

	float current_tick_max_delta = 0.0f;
	float previous_jitter_lby = 0.0f;
	int previous_resolving = 0;
	float tick_delta = 0.0f;
	float lby_delta = 0.0f;
	float abs_yaw_delta = 0.0f;
	float last_velocity_angle = 0.0f;
	float last_speed = 0.0f;
	float last_lby = 0.0f;

	bool simtime_updated = false;
	bool inverse_lby = false;
	bool did_hit_inversed_lby = false;

	float left_side = FLT_MAX;
	float right_side = FLT_MAX;
	float left_lside = FLT_MAX;
	float lby_side = FLT_MAX;
	float right_lside = FLT_MAX;
	float no_side = FLT_MAX;

	//float next_predicted_lby_update;
	//float last_lby_update;
	//bool did_predicted_lby_flick;

	/*float last_time_down_pitch;
	float last_time_lby_updated;
	int lby_update_tick;*/
	bool skeet_fakes = false;
	bool is_shifting = false;
	bool was_shifting = false;
	bool is_jittering = false;
	bool is_using_static = false;
	bool is_using_balance = false;
	bool did_lby_update_fail = false;

	bool force_resolving = false;
	std::string resolver_method_text;

	float m_flRate = 0.0f;

	float m_flClientRate = 0.0f;
	float prev_delta = FLT_MAX;
	float m_flLastFlickTime = FLT_MAX;
	int m_iSide = -1;
	int m_iCurrentSide = -1;
	bool b_bRateCheck = false;

	bool animations_updated = false;

	bool invalidate_resolving = false;
	int resolving_method;
	int cfirstmisses = 0;
	int missed_shots[12];

	float last_anims_update_time;

	matrix3x4_t leftmx[128];
	matrix3x4_t rightmx[128];
	matrix3x4_t leftlmx[128];
	matrix3x4_t rightlmx[128];
	matrix3x4_t LBYmx[128];
	matrix3x4_t nodesmx[128];

	/*bool lby_update_locked;*/
	bool is_dormant, resolved;
	//bool /*is_last_moving_lby_valid, last_moving_lby_can_be_invalid, is_just_stopped, is_getting_right_delta,*/is_breaking_lby;
	/*bool is_fakewalking;
	bool is_balance_adjust_triggered, is_balance_adjust_playing;
	bool did_lby_flick;*/
	bool fakeangles;
	//bool did_hit_low_delta;
	//bool did_hit_max_delta;
	//bool did_hit_no_delta;
};

struct resolverInfo_t {
	resolverInfo_t() = default;

	struct missInfo_t {
		missInfo_t() {
			this->m_freestand = 0;
			this->m_first_shot = 0;
			this->m_static = 0;
			this->m_jitter = 0;
		}

		int m_freestand;
		int m_first_shot;
		int m_static;
		int m_jitter;
	};

	struct hitInfo_t {
		hitInfo_t() {
			this->m_freestand = 0;
			this->m_first_shot = 0;
			this->m_static = 0;
			this->m_jitter = 0;
		}

		int m_freestand;
		int m_first_shot;
		int m_static;
		int m_jitter;
	};

	bool m_jitter{};
	bool m_use_freestand{};

	missInfo_t m_misses{};
	hitInfo_t m_hits{};
};

class Resolver {
public:
	enum Modes : size_t {
		RESOLVE_NONE = 0,
		RESOLVE_MOVE,
		RESOLVE_STAND,
	};

public:
	void ResolveAngles(Player* player, LagComp::LagRecord_t* record);
	void ResolveStand(AimPlayer* data, LagComp::LagRecord_t* record);
	float ResolveShot(AimPlayer* data, LagComp::LagRecord_t* record);
	bool has_fake(Player* player, LagComp::LagRecord_t* record);
	//void store_delta(Player* m_player, resolver_recode* resolve_data, LagComp::LagRecord_t* m_records);
	//void update_animations_for_resolver(Player* player, LagComp::LagRecord_t* from, int Resolver_Index);
	resolver_recode inform[64];

public:
	std::array< vec3_t, 64 > m_impacts;
	//std::unordered_map<CBaseHandle, resolverInfo_t> m_resolver_info;
};

extern Resolver g_resolver;