#pragma once
enum m_ent_list {
	m_enemy,
	m_team,
	m_local,
	m_hands,
	m_weapon,
	m_size
};

class value {
public:
	template <typename t = float>
	t get() {
		return (t)m_value;
	}

	template <typename t = float>
	void set(t in) {
		m_value = (double)in;
	}

	Color get_color() {
		int a = ( (D3DCOLOR)m_value >> 24 ) & 0xff;
		int r = ( ( D3DCOLOR ) m_value >> 16 ) & 0xff;
		int g = ( ( D3DCOLOR ) m_value >> 8 ) & 0xff;
		int b = ( D3DCOLOR ) m_value & 0xff;

		if (a == 0 && r == 0 && g == 0 && b == 0) {
			a = 255;
			r = 255;
			g = 255;
			b = 255;
		}

		Color ret( r, g, b, a );
		return ret;
	}

	void set_color(Color in) {
		DWORD color = D3DCOLOR_RGBA(in.r(), in.g(), in.b(), in.a());
		
		m_value = (double)color;
	}


private:
	double m_value = 0.0;
};

extern std::map< std::string, value > g_cfg;

class zaebal
{
public:
	class ConfigThings
	{
	public:
		bool Save(std::string file_name);
		bool Load(std::string file_name);
		bool Remove(std::string file_name);
		void CreateConfig(std::string name);
		std::vector<std::string> GetConfigs();
	}Config;
	class rage
	{
	public:
		bool rage_aimbot_enabled;
		bool rage_aimbot_limit_fov;
		bool rage_aimbot_silent_aim;
		bool rage_aimbot_autofire;
		bool rage_aimbot_ignore_limbs;
		bool rage_aimbot_prefer_safepoint;
		bool rage_aimbot_scan_history_rec;
		bool rage_aimbot_hitchance;
		bool rage_aimbot_minimal_damage_hp;
		bool rage_aimbot_penetration;
		bool rage_aimbot_penetration_minimal_damage_hp;
		bool rage_aimbot_minimal_damage_override_pen;
		bool misc_knifebot;
		bool aimbot_hitbox[7] = { false, false, false, false, false, false, false };
		bool aimbot_multipoint[5] = { false, false, false, false, false };
		bool rage_aimbot_norecoil;
		bool aimbot_baim_prefer[2] = { false, false };

		float rage_aimbot_limit_fov_amount = 0.f;
		float rage_aimbot_multipoint_scale = 0.f;
		float rage_aimbot_stomach_scale = 0.f;
		float rage_aimbot_hitchance_amount = 1.f;
		float rage_aimbot_minimal_damage = 1.f;
		float rage_aimbot_penetration_minimal_damage = 0.f;
		float rage_aimbot_minimal_damage_override = 1.f;

		bool exploit_xx;

		int rage_exploit_key = 0;
		int rage_exploit_key_style = 2;
		int rage_exploit_type;
		int rage_aimbot_target_selection;
		int rage_aimbot_prefered_hitbox;
		int rage_aimbot_minimal_damage_override_key = 0;
		int rage_aimbot_minimal_damage_override_key_style = 1;
		int rage_aimbot_auto_scope;
		int rage_aimbot_auto_stop;
		int rage_aimbot_baim_key = 0;
		int rage_aimbot_baim_key_style = 2;


		bool rage_aa_enabled;
		int rage_aa_pitch;
		int rage_aa_yaw_base;
		int rage_aa_yaw;
		int rage_aa_yaw_direction;
		float rage_aa_random_update = 0.f;
		int rage_aa_jitter_type;
		float rage_aa_jitter_range = 0.f;
		bool rage_aa_rotation_enabled;
		float rage_aa_rotation_range = 1.f;
		float rage_aa_rotation_speed = 0.f;
		int rage_fakelag_type;
		float rage_fakelag_limit = 2.f;
		bool rage_fakelag_enabled;
		bool fakelag[4] = { false, false, false, false };
		int rage_aa_yaw_fake;
		bool rage_aa_real_around_fake[4] = { false, false, false, false };
		bool rage_aa_real_around_fake_jitter;
		float rage_aa_custom_fake = -60.f;
		int rage_aa_negate_key = 0;
		int rage_aa_negate_key_style = 2;
	}rage;
	class player_esp
	{
	public:
		bool name;
		Color name_color = Color(255, 255, 255);
		bool name_team;
		Color name_team_color = Color(255, 255, 255);
		bool enemies_dormant;
		bool enemies_box;
		Color enemies_box_color = Color( 255, 255, 255 );

		/* hue */
		float hue_box = 100.f;
		float hue_name = 100.f;
		float hue_weapon = 100.f;
		float hue_ammo = 100.f;
		float hue_health = 100.f;
		float hue_offscreen = 100.f;

		bool team_box;
		Color team_box_color = Color(255, 255, 255);
		bool enemies_skeleton;
		Color enemies_skeleton_color = Color(255, 255, 255);
		bool team_skeleton;
		Color team_skeleton_color = Color(255, 255, 255);
		bool enemies_offscreen;
		Color enemies_offscreen_color = Color(255, 255, 255);
		bool enemies_health;
		Color enemies_health_color = Color(255, 255, 255);
		bool enemies_health_override;
		bool team_health;
		bool enemies_weapon;
		Color enemies_weapon_color = Color(255, 255, 255);
		bool team_weapon;
		Color team_weapon_color = Color(255, 255, 255);
		int enemies_weapon_type;
		int team_weapon_type;
		bool enemies_flags[4] = { false, false, false, false };
		bool enemies_ammo;
		Color enemies_ammo_color = Color(255, 255, 255);
		float box_horizontal = 100.f;
		float box_vertical = 100.f;
		bool box_outline;
		bool enemy_glow;

		bool desync_chams;
		Color desync_chams_color = Color( 255, 255, 255 );
		int desync_chams_material;
		float hue_desync;

		Color enemy_glow_color = Color(255, 255, 255);
		bool chams_enemies;
		Color chams_enemies_color = Color(255, 255, 255);
		int chams_enemies_material;
		bool chams_enemies_invis;
		Color chams_enemies_invis_color = Color(255, 255, 255);
		int chams_enemies_invis_material;
		bool chams_enemy_backtrack;
		Color chams_enemy_backtrack_color = Color(255, 255, 255);
		int group_chams;
		int chams_local_material;
		bool local_glow;
		Color local_glow_color = Color(255, 255, 255);
		bool chams_local_enable;
		Color chams_local_color = Color(255, 255, 255);
		bool team_glow;
		Color team_glow_color = Color(255, 255, 255);
		bool chams_team;
		Color chams_team_color = Color(255, 255, 255);
		int chams_team_material;
		bool chams_team_invis;
		Color chams_team_invis_color = Color(255, 255, 255);
		int chams_team_invis_material;
		bool remove_scope;
		bool remove_visual_recoil;
		bool remove_smoke;
		bool remove_flash;
		bool remove_fog;
		bool no_draw_team;
		bool grenade_prediction;
		Color grenade_prediction_color1 = Color(255, 255, 255);
		bool world[2] = { false, false };
		float world_night_darkness = 0.f;
		float world_prop_opacity = 0.f;
		bool bullet_beam;
		int bullet_beam_type;
		float bullet_beam_time = 0.f;
		Color bullet_beam_color = Color(255, 255, 255);
		Color bullet_beam_hurt_color = Color(255, 255, 255);
		bool bomb[3] = { false, false, false };
		bool projectiles;
		Color projectiles_color = Color(255, 255, 255);
		bool force_crosshair;
		bool penetration_crosshair;
		bool enemies_radar;
	} player_esp;

	class miscellaneous
	{
	public:
		bool bhop;
		bool air_duck;
		int thirdperson_key = 0;
		int thirdperson_key_style = 2;
		int fakeduck_key = 0;
		int fakeduck_key_style = 1;
		bool thirdperson;
		float thirdperson_distance = 100.f;
		bool auto_strafe;
		bool wasd_strafe;
		bool override_fov;
		float override_fov_amount = 90.f;
		bool override_fov_scoped;
		bool clantag;
		int hitmarker;
		int hitsound;
		int custom_hitsound;
		float hitsound_volume = 0.f;
		bool log[5] = { false, false, false, false, false };
		bool override_viewmodel_fov;
		float override_viewmodel_fov_amount = 60.f;
		bool spectators;
		bool preserve_killfeed;
		bool ragdoll_force;
		bool unlock_inventory;
	} miscellaneous;

};
extern zaebal g_loser;

class Config {
public:
	void init( );

	inline static std::unordered_map<std::string, bool> m_hotkey_states{};

	bool get_hotkey(std::string str);

	void save(std::string name = XOR("config.ini"), bool to_clipboard = false);
	void load(std::string name = XOR("config.ini"), bool from_clipboard = false);

private:
	bool m_init;
	std::string m_path;
};

extern Config g_config;