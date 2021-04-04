#include <filesystem>
#include "m_elements.h"
#include "new_framework/m_elements/m_window/m_window.h"
#include "new_framework/m_elements/m_button/m_button.h"
#include "new_framework/m_elements/m_checkbox/m_checkbox.h"
#include "new_framework/m_elements/m_listbox/m_listbox.h"
#include "new_framework/m_elements/m_textinput/m_textinput.h"
#include "new_framework/m_elements/m_colorpicker/m_colorpicker.h"
#include "new_framework/m_elements/m_combo/m_combo.h"
#include "new_framework/m_elements/m_multi/m_multi.h"
#include "new_framework/m_elements/m_slider/m_slider.h"
#include "new_framework/m_elements/m_binds/m_binds.h"
#include "new_framework/m_elements/m_colorpicker/m_color_t/m_color_t.h"

/* gui externals related */
Gui::Details Gui::m_Details;
Gui::Input Gui::m_Input;
Gui::Control Gui::m_Control;
Gui::ExternalRendering Gui::m_External;
Gui::Config Gui::m_Cfg;

/* gui func related */
bool Gui::Details::GetMenuState( ) {
	return m_Show;
}
void Gui::Details::SetMenuState( const bool m_State ) {
	m_Show = m_State;
}
vec2_t Gui::Input::GetMousePos( ) {
	return m_MousePos;
}
void Gui::Input::SetMousePos( const vec2_t m_Pos ) {
	m_MousePos = vec2_t{ m_Pos.x, m_Pos.y };
}
void Gui::Input::PollInput( ) {
	for ( size_t i{ 0 }; i < 256; i++ ) {
		Helpers.m_PrevKeyState[ i ] = Helpers.m_KeyState[ i ];
		Helpers.m_KeyState[ i ] = GetAsyncKeyState( i );
	}
}
bool Gui::Input::KeyPressed( const uintptr_t m_Key ) {
	return Helpers.m_KeyState[ m_Key ] && !Helpers.m_PrevKeyState[ m_Key ];
}
bool Gui::Input::MousePointer( const vec2_t m_Pos, const vec2_t m_Size ) {
	std::pair<bool, bool> m_ShouldReturn {
		GetMousePos( ).x > m_Pos.x && GetMousePos( ).y > m_Pos.y,
		GetMousePos( ).x < m_Pos.x + m_Size.x && GetMousePos( ).y < m_Pos.y + m_Size.y
	};

	return m_ShouldReturn.first && m_ShouldReturn.second;
}
void Gui::Input::SetMouseWheel( const float m_Wheel ) {
	m_MouseWheel = m_Wheel;
}
float Gui::Input::GetMouseWheel( ) {
	return m_MouseWheel;
}
int Gui::Control::GetIndex( ) {
	return m_Index;
}
void Gui::Control::SetIndex( const uintptr_t m_Last ) {
	m_Index = m_Last;
}
bool Gui::Control::IsPossible( ) {
	return Gui::m_Control.m_Opened
		[ Gui::m_Control.ControlType::COMBO ] == -1 &&
		Gui::m_Control.m_Opened
		[ Gui::m_Control.ControlType::MULTICOMBO ] == -1 &&
		Gui::m_Control.m_Opened
		[ Gui::m_Control.ControlType::BIND ] == -1;
}

/* drawing related */
void GradientHorizontalPrototype2( vec2_t a, vec2_t b, Color c_a, Color c_b )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) ) }
	};

	DrawList.dev->SetTexture( 0, nullptr );
	DrawList.dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void GradientVerticalPrototype2( vec2_t a, vec2_t b, Color c_a, Color c_b )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) ) }
	};

	DrawList.dev->SetTexture( 0, nullptr );
	DrawList.dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void GradientVertical2( float x, float y, float w, float h, Color c_a, Color c_b )
{
	GradientVerticalPrototype2( { x, y }, { w, h }, c_a, c_b );
}
void Gui::ExternalRendering::GradientHorizontal2( float x, float y, float w, float h, Color c_a, Color c_b )
{
	GradientHorizontalPrototype2( { x, y }, { w, h }, c_a, c_b );
}
void FilledPrototype2( vec2_t a, vec2_t b, Color c )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) }
	};

	DrawList.dev->SetTexture( 0, nullptr );
	DrawList.dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void Gui::ExternalRendering::FilledRect2( float x, float y, float w, float h, Color c )
{
	FilledPrototype2( { x, y }, { w, h }, c );
}
void RectPrototype2( vec2_t a, vec2_t b, Color c )
{
	b += a;

	b.x -= 1;
	b.y -= 1;

	VerticeT verts[ 5 ] = {
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r(), c.g(), c.b(), c.a() ) },
		{ float( b.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r(), c.g(), c.b(), c.a() ) },
		{ float( b.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r(), c.g(), c.b(), c.a() ) },
		{ float( a.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r(), c.g(), c.b(), c.a() ) },
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r(), c.g(), c.b(), c.a() ) }
	};

	DrawList.dev->SetTexture( 0, nullptr );
	DrawList.dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &verts, 20 );
}
void Rect2( float x, float y, float w, float h, Color c )
{
	RectPrototype2( { x, y }, { w + 1, h + 1 }, c );
}

/* install */
void Gui::ExternalRendering::Install( ) {
	for ( size_t i{ 0 }; i < RectDraw.size( ); i++ )
		DrawList.FilledRect( RectDraw[ i ].posX, RectDraw[ i ].posY, RectDraw[ i ].m_Width, RectDraw[ i ].m_Height, RectDraw[ i ].m_color_t );

	for ( size_t i{ 0 }; i < RectDraw2.size( ); i++ )
		FilledRect2( RectDraw2[ i ].posX, RectDraw2[ i ].posY, RectDraw2[ i ].m_Width, RectDraw2[ i ].m_Height, RectDraw2[ i ].m_color_t );

	for ( size_t i{ 0 }; i < GradientHDraw.size( ); i++ )
		DrawList.GradientHorizontal( GradientHDraw[ i ].posX, GradientHDraw[ i ].posY, GradientHDraw[ i ].m_Width, GradientHDraw[ i ].m_Height, GradientHDraw[ i ].m_color_t, GradientHDraw[ i ].m_color_tNext );

	for ( size_t i{ 0 }; i < GradientVDraw.size( ); i++ )
		DrawList.GradientVertical( GradientVDraw[ i ].posX, GradientVDraw[ i ].posY, GradientVDraw[ i ].m_Width, GradientVDraw[ i ].m_Height, GradientVDraw[ i ].m_color_t, GradientVDraw[ i ].m_color_t );

	for ( size_t i{ 0 }; i < GradientHDraw2.size( ); i++ )
		GradientHorizontal2( GradientHDraw2[ i ].posX, GradientHDraw2[ i ].posY, GradientHDraw2[ i ].m_Width, GradientHDraw2[ i ].m_Height, GradientHDraw2[ i ].m_color_t, GradientHDraw2[ i ].m_color_tNext );

	for ( size_t i{ 0 }; i < GradientVDraw2.size( ); i++ )
		GradientVertical2( GradientVDraw2[ i ].posX, GradientVDraw2[ i ].posY, GradientVDraw2[ i ].m_Width, GradientVDraw2[ i ].m_Height, GradientVDraw2[ i ].m_color_t, GradientVDraw2[ i ].m_color_t );

	for ( size_t i{ 0 }; i < OutlineDraw.size( ); i++ )
		DrawList.Rect( OutlineDraw[ i ].posX, OutlineDraw[ i ].posY, OutlineDraw[ i ].m_Width, OutlineDraw[ i ].m_Height, OutlineDraw[ i ].m_color_t );

	for ( size_t i{ 0 }; i < OutlineDraw2.size( ); i++ )
		Rect2( OutlineDraw2[ i ].posX, OutlineDraw2[ i ].posY, OutlineDraw2[ i ].m_Width, OutlineDraw2[ i ].m_Height, OutlineDraw2[ i ].m_color_t );

	for ( size_t i{ 0 }; i < TextDraw.size( ); i++ )
		DrawList.DrawString( { TextDraw[ i ].posX, TextDraw[ i ].posY }, TextDraw[ i ].m_Text, TextDraw[ i ].m_color_t, Fonts::Main, font_flags::drop_shadow );

	/* clear */
	RectDraw.clear( );
	OutlineDraw.clear( );
	GradientHDraw.clear( );
	GradientVDraw.clear( );
	TextDraw.clear( );

	/* clear */
	RectDraw2.clear( );
	OutlineDraw2.clear( );
	GradientHDraw2.clear( );
	GradientVDraw2.clear( );
}

/* config related */
static int current_preset;
std::vector<std::string> presets = { "slot 1", "slot 2" };

/* load config */
void Gui::Config::LoadConfig( ) {
	if ( g_loser.Config.Load( presets[ current_preset ] ) ) {
		g_notify.add( XOR( "[ stellarcheat ] Config loaded succesfully\n" ) );
	}
	else {
		g_notify.add( XOR( "[ stellarcheat ] Loading config problem, try again\n" ) );
	}
}

/* save config */
void Gui::Config::SaveConfig( ) {
	/* creating config then saving it */
	g_loser.Config.CreateConfig( presets[ current_preset ] );
	if ( g_loser.Config.Save( presets[ current_preset ] ) ) {
		g_notify.add( XOR( "[ stellarcheat ] Config saved succesfully\n" ) );
	}
	else {
		g_notify.add( XOR( "[ stellarcheat ] Saving config problem, try again\n" ) );
	}
}

/* structs related */
std::vector<std::string> tabs = { "Ragebot", "Anti-Aim", "Visuals", "Settings", "Skins" };
std::vector<std::string> hah = { "None" };

/* ragebot strings */
std::vector<std::string> aimbot_target_selection = { "lowest distance", "nearest crosshair", "higher damage", "lowest healt", "lowest hitbox ( bad pc )", "best entity position" };
std::vector<std::string> aimbot_prefered_hitbox = { "head", "body", "stomach" };
std::vector<std::string> aimbot_automatic_stop = { "lowest damage", "fast", "enemy visible" };
std::vector<std::string> aimbot_automatic_scope = { "disabled", "always autoscope", "low accuracy" };
std::vector<std::string> antiaim_fakelag_mode = { "factor", "random ticks", "adaptive", "stepping" };
std::vector<std::string> aimbot_exploits = { "disabled", "double tap", "hide shots" };

/* antiaim */
std::vector<std::string> antiaim_pitch = { "disabled", "emotion", "up", "nospread", "ideal pitch" };
std::vector<std::string> antiaim_static_angle = { "disabled", "low crosshair", "low distance" };
std::vector<std::string> antiaim_mode = { "disabled", "automatic direction", "offset based" };

/* visuals strings */
std::vector<std::string> visuals_weapon_mode = { "text", "icon", "text & icon" };
std::vector<std::string> visuals_chams = { "enemy", "local" };
std::vector<std::string> visuals_chams_material = { "csgo model", "default", "flat", "armsrace", "glow", "glass", "crystal" };

vec2_t window_position = vec2_t( 0, 0 );

/* install ui */
void Gui::Details::Install( ) {
	/* pool */
	m_Input.PollInput( );

	/* menu open related */
	if ( m_Input.KeyPressed( VK_INSERT ) ) {
		SetMenuState( !GetMenuState( ) );
	}

	if ( !GetMenuState( ) )
		return;

	m_Control.SetIndex( 0 );
	m_Control.m_Opened[ m_Control.ControlType::COMBO ] = -1;
	m_Control.m_Opened[ m_Control.ControlType::MULTICOMBO ] = -1;
	m_Control.m_Opened[ m_Control.ControlType::BIND ] = -1;

	auto m_Window = new Window( "stellarcheat", &window_position, { 475, 355 }, tabs, &m_Index ); {
		auto rage_main = new Groupbox( "rage main", 0, 19, 45, 208, 125, 0 );
		{
			m_Window->AddGroup( rage_main );
			rage_main->AddElement( new Checkbox( "enable ragebot", &g_loser.rage.rage_aimbot_enabled ) );
			rage_main->AddElement( new Combo( "target selection", &g_loser.rage.rage_aimbot_target_selection, aimbot_target_selection ) );
			rage_main->AddElement( new Checkbox( "silent aim", &g_loser.rage.rage_aimbot_silent_aim ) );
			rage_main->AddElement( new Checkbox( "automatic fire", &g_loser.rage.rage_aimbot_autofire ) );
			rage_main->AddElement( new Checkbox( "automatic wall", &g_loser.rage.rage_aimbot_penetration ) );
			rage_main->AddElement( new Checkbox( "automatic knife", &g_loser.rage.misc_knifebot ) );
			rage_main->AddElement( new Checkbox( "remove weapon recoil", &g_loser.rage.rage_aimbot_norecoil ) );
			
			/* initialize scrollbars */
			rage_main->InitScroll( );
		} delete rage_main;

		auto rage_accuracy = new Groupbox( "accuracy", 1, 250, 45, 208, 270, 0 );
		{
			m_Window->AddGroup( rage_accuracy );
			rage_accuracy->AddElement( new Checkbox( "accuracy elements", &g_loser.rage.rage_aimbot_hitchance ) );
			if ( g_loser.rage.rage_aimbot_hitchance ) {
				rage_accuracy->AddElement( new Slider( "hitchance ammount", &g_loser.rage.rage_aimbot_hitchance_amount, 0, 100, "%" ) );
				rage_accuracy->AddElement( new Slider( "minimum damage", &g_loser.rage.rage_aimbot_minimal_damage, 0, 100, "%" ) );
				if ( g_loser.rage.rage_aimbot_penetration ) {
					rage_accuracy->AddElement( new Slider( "autowall damage", &g_loser.rage.rage_aimbot_penetration_minimal_damage, 0, 100, "%" ) );
				}		

				/* scales */
				rage_accuracy->AddElement( new Slider( "multipoint rate", &g_loser.rage.rage_aimbot_multipoint_scale, 0, 100, "%" ) );
				rage_accuracy->AddElement( new Slider( "stomach rate", &g_loser.rage.rage_aimbot_stomach_scale, 0, 100, "%" ) );
			}

			/* external */
			rage_accuracy->AddElement( new Combo( "automatic stop", &g_loser.rage.rage_aimbot_auto_stop, aimbot_automatic_stop ) );
			rage_accuracy->AddElement( new Combo( "automatic scope", &g_loser.rage.rage_aimbot_auto_scope, aimbot_automatic_scope ) );

			/* prefer bodyaim */
			auto prefer_bodyaim = new Multi( "prefer body conditions" );
			prefer_bodyaim->AddItem( "lethal", &g_loser.rage.aimbot_baim_prefer[ 0 ] );
			prefer_bodyaim->AddItem( "enemy in air", &g_loser.rage.aimbot_baim_prefer[ 1 ] );
			rage_accuracy->AddElement( prefer_bodyaim );

			rage_accuracy->AddElement( new Checkbox( "ignore legs when moving", &g_loser.rage.rage_aimbot_ignore_limbs ) );
			rage_accuracy->AddElement( new Checkbox( "prefer safepoint", &g_loser.rage.rage_aimbot_prefer_safepoint ) );

			rage_accuracy->AddElement( new Combo( "exploit selection", &g_loser.rage.rage_exploit_type, aimbot_exploits ) );
			if ( g_loser.rage.rage_exploit_type > 0 ) {
				rage_accuracy->AddElement( new Checkbox( "prepare exploit", &g_loser.rage.exploit_xx ) );
				rage_accuracy->AddElement( new Bind( &g_loser.rage.rage_exploit_key, &g_loser.rage.rage_exploit_key_style ) );
			}

			/* initialize scrollbars */
			rage_accuracy->InitScroll( );
		} delete rage_accuracy;

		auto rage_selection = new Groupbox( "hitbox selection", 2, 19, 199, 208, 115, 0 );
		{
			m_Window->AddGroup( rage_selection );
			rage_selection->AddElement( new Combo( "prefered hitbox", &g_loser.rage.rage_aimbot_prefered_hitbox, aimbot_prefered_hitbox ) );
			
			/* hitboxes */
			auto hitbox_selection = new Multi( "hitboxes" );
			hitbox_selection->AddItem( "head", &g_loser.rage.aimbot_hitbox[ 0 ] );
			hitbox_selection->AddItem( "neck", &g_loser.rage.aimbot_hitbox[ 1 ] );
			hitbox_selection->AddItem( "chest", &g_loser.rage.aimbot_hitbox[ 2 ] );
			hitbox_selection->AddItem( "stomach", &g_loser.rage.aimbot_hitbox[ 3 ] );
			hitbox_selection->AddItem( "arms", &g_loser.rage.aimbot_hitbox[ 4 ] );
			hitbox_selection->AddItem( "legs", &g_loser.rage.aimbot_hitbox[ 5 ] );
			hitbox_selection->AddItem( "feet", &g_loser.rage.aimbot_hitbox[ 6 ] );
			rage_selection->AddElement( hitbox_selection );

			/* multipoint */
			auto multipoint_selection = new Multi( "multipoint" );
			multipoint_selection->AddItem( "head", &g_loser.rage.aimbot_multipoint[ 0 ] );
			multipoint_selection->AddItem( "chest", &g_loser.rage.aimbot_multipoint[ 1 ] );
			multipoint_selection->AddItem( "stomach", &g_loser.rage.aimbot_multipoint[ 2 ] );
			multipoint_selection->AddItem( "legs", &g_loser.rage.aimbot_multipoint[ 3 ] );
			multipoint_selection->AddItem( "feet", &g_loser.rage.aimbot_multipoint[ 4 ] );
			rage_selection->AddElement( multipoint_selection );

			/* initialize scrollbars */
			rage_selection->InitScroll( );
		} delete rage_selection;

		/* antiaim tab */
		auto m_antiaim_m = new Groupbox( "antiaim main", 3, 19, 45, 208, 270, 1 );
		{
			m_Window->AddGroup( m_antiaim_m );
			m_antiaim_m->AddElement( new Checkbox( "enable antiaim", &g_loser.rage.rage_aa_enabled ) );
			m_antiaim_m->AddElement( new Combo( "antiaim mode", &g_loser.rage.rage_aa_yaw, antiaim_mode ) );
			m_antiaim_m->AddElement( new Combo( "pitch", &g_loser.rage.rage_aa_pitch, antiaim_pitch ) );
			m_antiaim_m->AddElement( new Combo( "prefered angle", &g_loser.rage.rage_aa_yaw_base, antiaim_static_angle ) );

		} delete m_antiaim_m;

		auto m_antiaim_o = new Groupbox( "desync", 4, 250, 45, 208, 95, 1 );
		{
			m_Window->AddGroup( m_antiaim_o );


		} delete m_antiaim_o;

		auto m_antiaim_fakelag = new Groupbox( "other", 5, 250, 175, 208, 140, 1 );
		{
			m_Window->AddGroup( m_antiaim_fakelag );
			m_antiaim_fakelag->AddElement( new Checkbox( "enable fakelag", &g_loser.rage.rage_fakelag_enabled ) );
			if ( g_loser.rage.rage_fakelag_enabled ) {
				/* fakelag activation mode */
				auto m_fakelag_activation = new Multi( "fakelag activation" );
				m_fakelag_activation->AddItem( "standing", &g_loser.rage.fakelag[ 0 ] );
				m_fakelag_activation->AddItem( "moving", &g_loser.rage.fakelag[ 1 ] );
				m_fakelag_activation->AddItem( "in air", &g_loser.rage.fakelag[ 2 ] );
				m_fakelag_activation->AddItem( "inaccurate fix", &g_loser.rage.fakelag[ 3 ] );
				m_antiaim_fakelag->AddElement( m_fakelag_activation );

				m_antiaim_fakelag->AddElement( new Slider( "fakelag ticks", &g_loser.rage.rage_fakelag_limit, 0, 16, "%" ) );
				m_antiaim_fakelag->AddElement( new Combo( "fakelag mode", &g_loser.rage.rage_fakelag_type, antiaim_fakelag_mode ) );
			}

			m_antiaim_fakelag->AddElement( new Checkbox( "fakeduck", &g_loser.player_esp.fakeduck_x ) );
			if ( g_loser.player_esp.fakeduck_x ) {
				m_antiaim_fakelag->AddElement( new Bind( &g_loser.miscellaneous.fakeduck_key, &g_loser.miscellaneous.fakeduck_key_style ) );
			}
			m_antiaim_fakelag->AddElement( new Checkbox( "slowwalk", &g_loser.player_esp.slowwalk_x ) );
			if ( g_loser.player_esp.slowwalk_x ) {
				m_antiaim_fakelag->AddElement( new Bind( &g_loser.rage.slowwalk_key, &g_loser.rage.slowwalk_key_style ) );
				m_antiaim_fakelag->AddElement( new Slider( "slowwalk value", &g_loser.rage.slowwalk_value, 0, 40, "%" ) );
			}
			/* initialize scroll */
			m_antiaim_fakelag->InitScroll( );
		} delete m_antiaim_fakelag;

		/* visuals tab */
		auto m_visuals_players = new Groupbox( "players", 6, 19, 45, 208, 270, 2 );
		{
			m_Window->AddGroup( m_visuals_players );
			m_visuals_players->AddElement( new Checkbox( "dormant esp", &g_loser.player_esp.enemies_dormant ) );
			
			/* bounding box */
			m_visuals_players->AddElement( new Checkbox( "bounding box", &g_loser.player_esp.enemies_box ) );
			if ( g_loser.player_esp.enemies_box ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_box_color, &g_loser.player_esp.hue_box ) );
				m_visuals_players->AddElement( new Checkbox( "box outline", &g_loser.player_esp.box_outline ) );
			}

			/* healthbar */
			m_visuals_players->AddElement( new Checkbox( "healthbar", &g_loser.player_esp.enemies_health ) );
			if ( g_loser.player_esp.enemies_health ) {
				m_visuals_players->AddElement( new Checkbox( "healthbar color override", &g_loser.player_esp.enemies_health_override ) );
				if ( g_loser.player_esp.enemies_health_override ) {
					m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_health_color, &g_loser.player_esp.hue_health ) );
				}
			}

			/* name esp */
			m_visuals_players->AddElement( new Checkbox( "name", &g_loser.player_esp.name ) );
			if ( g_loser.player_esp.name ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.name_color, &g_loser.player_esp.hue_name ) );
			}
			
			/* weapon esp */
			m_visuals_players->AddElement( new Checkbox( "weapon", &g_loser.player_esp.enemies_weapon ) );
			if ( g_loser.player_esp.enemies_weapon ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_weapon_color, &g_loser.player_esp.hue_weapon ) );
				m_visuals_players->AddElement( new Combo( "weapon type", &g_loser.player_esp.enemies_weapon_type, visuals_weapon_mode ) );
			}

			/* ammo esp */
			m_visuals_players->AddElement( new Checkbox( "ammo", &g_loser.player_esp.enemies_ammo ) );
			if ( g_loser.player_esp.enemies_ammo ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_ammo_color, &g_loser.player_esp.hue_ammo ) );
			}

			/* skeleton esp */
			m_visuals_players->AddElement( new Checkbox( "skeleton", &g_loser.player_esp.enemies_skeleton ) );
			if ( g_loser.player_esp.enemies_skeleton ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_skeleton_color, &g_loser.player_esp.hue_skeleton ) );
			}

			m_visuals_players->AddElement( new Checkbox( "offscreen arrows", &g_loser.player_esp.enemies_offscreen ) );
			if ( g_loser.player_esp.enemies_offscreen ) {
				m_visuals_players->AddElement( new ColorPicker( &g_loser.player_esp.enemies_offscreen_color, &g_loser.player_esp.hue_offscreen ) );
			}

			/* flags */
			auto m_flags = new Multi( "flags" );
			m_flags->AddItem( "money", &g_loser.player_esp.enemies_flags[ 0 ] );
			m_flags->AddItem( "armor", &g_loser.player_esp.enemies_flags[ 1 ] );
			m_flags->AddItem( "scoped", &g_loser.player_esp.enemies_flags[ 2 ] );
			m_flags->AddItem( "bomb", &g_loser.player_esp.enemies_flags[ 3 ] );
			m_visuals_players->AddElement( m_flags );

			/* initialize scrollbar*/
			m_visuals_players->InitScroll( );
		} delete m_visuals_players;

		auto m_visuals_chams = new Groupbox( "chams", 7, 250, 45, 208, 95, 2 );
		{ 
			m_Window->AddGroup( m_visuals_chams );
			m_visuals_chams->AddElement( new Combo( "entity", &g_loser.player_esp.group_chams, visuals_chams ) );
			if ( g_loser.player_esp.group_chams == 0 ) {
				/* entity */
				m_visuals_chams->AddElement( new Checkbox( "glow", &g_loser.player_esp.enemy_glow ) );
				if ( g_loser.player_esp.enemy_glow ) {
					m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.enemy_glow_color, &g_loser.player_esp.hue_glowe, true ) );
				}

				m_visuals_chams->AddElement( new Checkbox( "enemy chams", &g_loser.player_esp.chams_enemies ) );
				if ( g_loser.player_esp.chams_enemies ) {
					m_visuals_chams->AddElement( new Checkbox( "enemy double", &g_loser.player_esp.chams_enemies_invis ) );
					if ( g_loser.player_esp.chams_enemies_invis ) {
						m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.chams_enemies_color, &g_loser.player_esp.hue_enech, true ) );
						m_visuals_chams->AddElement( new Checkbox( "double fix ( aleardy on )", &g_loser.player_esp.chams_enemies_invis ) );
						m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.chams_enemies_invis_color, &g_loser.player_esp.hue_eneich, true ) );
						m_visuals_chams->AddElement( new Combo( "enemy chams materials", &g_loser.player_esp.chams_enemies_invis_material, visuals_chams_material ) );
					}
				}

			}
			else if ( g_loser.player_esp.group_chams == 1 ) {
				/* local */
				m_visuals_chams->AddElement( new Checkbox( "glow", &g_loser.player_esp.local_glow ) );
				if ( g_loser.player_esp.local_glow ) {
					m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.local_glow_color, &g_loser.player_esp.hue_glowl, true ) );
				}

				m_visuals_chams->AddElement( new Checkbox( "local chams", &g_loser.player_esp.chams_local_enable ) );
				if ( g_loser.player_esp.chams_local_enable ) {
					m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.chams_local_color, &g_loser.player_esp.hue_localch, true ) );
					m_visuals_chams->AddElement( new Combo( "local chams materials", &g_loser.player_esp.chams_local_material, visuals_chams_material ) );
				}

				m_visuals_chams->AddElement( new Checkbox( "desync chams", &g_loser.player_esp.desync_chams ) );
				if ( g_loser.player_esp.desync_chams ) {
					m_visuals_chams->AddElement( new ColorPicker( &g_loser.player_esp.desync_chams_color, &g_loser.player_esp.hue_desync, true ) );
					m_visuals_chams->AddElement( new Combo( "desync chams materials", &g_loser.player_esp.desync_chams_material, visuals_chams_material ) );
				}
			}

			/* initialize scrollbar */
			m_visuals_chams->InitScroll( );
		} delete m_visuals_chams;

		auto m_visuals_world = new Groupbox( "other", 8, 250, 175, 208, 140, 2 );
		{
			m_Window->AddGroup( m_visuals_world );

			/* night mode */
			auto m_world = new Multi( "world modulation" );
			m_world->AddItem( "nightmode", &g_loser.player_esp.world[ 0 ] );
			m_world->AddItem( "fullbright", &g_loser.player_esp.world[ 1 ] );
			m_visuals_world->AddElement( m_world );
			if ( g_loser.player_esp.world[ 0 ] ) {
				m_visuals_world->AddElement( new Slider( "darkness", &g_loser.player_esp.world_night_darkness, 0, 100, "%" ) );
			}

			m_visuals_world->AddElement( new Checkbox( "grenade prediction", &g_loser.player_esp.grenade_prediction ) );
			if ( g_loser.player_esp.grenade_prediction ) {
				m_visuals_world->AddElement( new ColorPicker( &g_loser.player_esp.grenade_prediction_color1, &g_loser.player_esp.hue_grenade_prediction, true ) );
			}

			m_visuals_world->AddElement( new Checkbox( "custom field of view", &g_loser.miscellaneous.override_fov ) );
			if ( g_loser.miscellaneous.override_fov ) {
				m_visuals_world->AddElement( new Slider( "field of view ammount", &g_loser.miscellaneous.override_fov_amount, 60, 140, "%" ) );
				m_visuals_world->AddElement( new Checkbox( "override zoom", &g_loser.miscellaneous.override_fov_scoped ) );
			}

			m_visuals_world->AddElement( new Checkbox( "remove scope", &g_loser.player_esp.remove_scope ) );
			m_visuals_world->AddElement( new Checkbox( "remove fog", &g_loser.player_esp.remove_fog ) );
			m_visuals_world->AddElement( new Checkbox( "remove flash", &g_loser.player_esp.remove_flash ) );
			m_visuals_world->AddElement( new Checkbox( "remove smoke", &g_loser.player_esp.remove_smoke ) );
			m_visuals_world->AddElement( new Checkbox( "remove visual recoil", &g_loser.player_esp.remove_visual_recoil ) );

			/* init scroll */
			m_visuals_world->InitScroll( );
		} delete m_visuals_world;

		/* settings tab */
		auto conf = new Groupbox( "misc", 9, 19, 45, 208, 270, 3 );
		{
			m_Window->AddGroup( conf );
			conf->AddElement( new Checkbox( "automatic bhop", &g_loser.miscellaneous.bhop ) );
			if ( g_loser.miscellaneous.bhop ) {
				conf->AddElement( new Checkbox( "duck in air", &g_loser.miscellaneous.air_duck ) );
			}
			conf->AddElement( new Checkbox( "automatic strafe", &g_loser.miscellaneous.auto_strafe ) );
			if ( g_loser.miscellaneous.auto_strafe ) {
				conf->AddElement( new Checkbox( "directional strafe", &g_loser.miscellaneous.wasd_strafe ) );
			}
			conf->AddElement( new Checkbox( "thirdperson", &g_loser.miscellaneous.thirdperson ) );
			if ( g_loser.miscellaneous.thirdperson ) {
				conf->AddElement( new Bind( &g_loser.miscellaneous.thirdperson_key, &g_loser.miscellaneous.thirdperson_key_style ) );
				conf->AddElement( new Slider( "thirdperson distance", &g_loser.miscellaneous.thirdperson_distance, 50, 150, "%" ) );
			}
			conf->AddElement( new Checkbox( "spectators list", &g_loser.miscellaneous.spectators ) );
			conf->AddElement( new Checkbox( "save killfeed", &g_loser.miscellaneous.preserve_killfeed ) );

		} delete conf;

		auto conf_scripts = new Groupbox( "other", 10, 250, 45, 208, 95, 3 );
		{
			m_Window->AddGroup( conf_scripts );
			conf_scripts->AddElement( new Checkbox( "menu color elements", &g_loser.menu.menu_color_1_ch ) );
			if ( g_loser.menu.menu_color_1_ch ) {
				conf_scripts->AddElement( new ColorPicker2( &g_loser.menu.menu_color_1, &g_loser.menu.menu_color_1_hue ) );

			}

		} delete conf_scripts;

		auto conf_func = new Groupbox( "configs", 11, 250, 175, 208, 140, 3 );
		{
			m_Window->AddGroup( conf_func );
			conf_func->AddElement( new Combo( "configs", &current_preset, presets ) );
			conf_func->AddElement( new Button( "save config", [ ]( ) { Gui::m_Cfg.SaveConfig( ); } ) );
			conf_func->AddElement( new Button( "load config", [ ]( ) { Gui::m_Cfg.LoadConfig( ); } ) );
		} delete conf_func;
	} delete m_Window;
	m_External.Install( );
}