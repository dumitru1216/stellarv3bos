#include "../gui.h"
#include "../form/form.h"

// .first  = regular rect
// .second = filled rect
std::pair<std::map<size_t, float>, std::map<size_t, float>> InAnimation;
std::pair<std::map<size_t, float>, std::map<size_t, float>> InTimer;

std::unordered_map<size_t, float> OutAnimation;
std::map<size_t, float> OutTimer;

std::map<size_t, float> CheckAnimation;
std::map<size_t, float> CheckTimer;

int alpha = 1;

bool GUI::Controls::Checkbox( const std::string &name, const std::string &value, bool DefaultVal, bool Unsafe ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( 8, 8 );

	if ( InAnimation.first.find( GUI::Hash( name ) ) == InAnimation.first.end( ) ) {
		InAnimation.first.insert( { GUI::Hash( name ), 0.f } );
		InTimer.first.insert( { GUI::Hash( name ), 0.f } );
	}

	if ( InAnimation.second.find( GUI::Hash( name ) ) == InAnimation.second.end( ) ) {
		InAnimation.second.insert( { GUI::Hash( name ), 0.f } );
		InTimer.second.insert( { GUI::Hash( name ), 0.f } );
	}

	if ( OutAnimation.find( GUI::Hash( name ) ) == OutAnimation.end( ) ) {
		OutAnimation.insert( { GUI::Hash( name ), 0.f } );
		OutTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if ( CheckAnimation.find( GUI::Hash( name ) ) == CheckAnimation.end( ) ) {
		CheckAnimation.insert( { GUI::Hash( name ), 0.f } );
		CheckTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	bool hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos,
		DrawSize + vec2_t(10 + D3D::GetTextSize(name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).x, 0) );


	// checkbox outline
	D3D::RectFilled(DrawPos, DrawSize, Color::Palette_t::MenuColors_t::Outlines().OverrideAlpha(100 * ctx->animation, true));


	if (hovered)
	{
		if (g_cfg[value].get<bool>())
		{
			D3D::RectFilled(DrawPos, DrawSize, g_cfg[XOR("menu_color")].get_color().OverrideAlpha(255 * ctx->animation, true));
		}
		else
		{
			D3D::RectFilled(DrawPos, DrawSize, g_cfg[XOR("menu_color")].get_color().OverrideAlpha(30 * ctx->animation, true));
		}
		//�����
	}
	else
	{
		//����� �����, �� �� �����?
		if (g_cfg[value].get<bool>())
		{
			D3D::RectFilled(DrawPos, DrawSize, g_cfg[XOR("menu_color")].get_color().OverrideAlpha(255 * ctx->animation, true));
		}
		else
		{
			D3D::RectFilled(DrawPos, DrawSize, g_cfg[XOR("menu_color")].get_color().OverrideAlpha(0 * ctx->animation, true));
		}
	}

	//�������� �� ����� �� ����


	// rect animation
	{
		if ( g_cfg[value].get<bool>() ) {
			InTimer.first.at( GUI::Hash( name ) ) += ( 1.0f / 0.15f ) * g_csgo.m_globals->m_frametime;
			InTimer.first.at( GUI::Hash( name ) ) = std::clamp<float>( InTimer.first.at( GUI::Hash( name ) ), 0.f, 1.f );

			InAnimation.first.at( GUI::Hash( name ) ) = Easing::OutCirc( InTimer.first.at( GUI::Hash( name ) ) );

			if ( InAnimation.first.at( GUI::Hash( name ) ) >= 0.999998 ) {
				InAnimation.first.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	// filled rect animation
	{
		if ( InAnimation.first.at( GUI::Hash( name ) ) >= 1.0f ) {
			InTimer.second.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			InTimer.second.at( GUI::Hash( name ) ) = std::clamp<float>( InTimer.second.at( GUI::Hash( name ) ), 0.f, 1.f );

			InAnimation.second.at( GUI::Hash( name ) ) = Easing::OutCirc( InTimer.second.at( GUI::Hash( name ) ) );

			if ( InAnimation.second.at( GUI::Hash( name ) ) >= 0.92f ) {
				InAnimation.second.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	// fade out
	{
		if ( !g_cfg[value].get<bool>()) {
			OutTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.25f ) * g_csgo.m_globals->m_frametime;
			OutTimer.at( GUI::Hash( name ) ) = std::clamp<float>( OutTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			OutAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( OutTimer.at( GUI::Hash( name ) ) );

			if ( OutAnimation.at( GUI::Hash( name ) ) >= 0.92f ) {
				InAnimation.first.at( GUI::Hash( name ) ) = 0.f;
				InAnimation.second.at( GUI::Hash( name ) ) = 0.f;

				InTimer.first.at( GUI::Hash( name ) ) = 0.f;
				InTimer.second.at( GUI::Hash( name ) ) = 0.f;
			}
		}
		else {
			OutAnimation.at( GUI::Hash( name ) ) = 0.f;
			OutTimer.at( GUI::Hash( name ) ) = 0.f;
		}
	}

	// check mark
	{
		if ( InAnimation.second.at( GUI::Hash( name ) ) >= 1.0f ) {
			CheckTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			CheckTimer.at( GUI::Hash( name ) ) = std::clamp<float>( CheckTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			CheckAnimation.at( GUI::Hash( name ) ) = Easing::OutCirc( CheckTimer.at( GUI::Hash( name ) ) );

			if ( CheckAnimation.at( GUI::Hash( name ) ) >= 0.999998 ) {
				CheckAnimation.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	vec2_t AnimationPos( ( ( DrawPos )+( ( DrawSize / 2 ) * ( 1.0f - InAnimation.first.at( GUI::Hash( name ) ) ) ) ) + 1 );
	vec2_t AnimationSize( ( ( ( DrawSize )*InAnimation.first.at( GUI::Hash( name ) ) ) + ( InAnimation.first.at( GUI::Hash( name ) ) < 0.999999 ? 1 : 0 ) ) - 2 );

	// initial animated rect, once this reaches 1.0 every other animation will follow



	


	D3D::Text( DrawPos + vec2_t( DrawSize.x + 5, -3 ), GUI::SplitStr( name, '#' )[ 0 ].data( ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ),
		D3D::EFontFlags::NONE );


	if (SHOULD_DRAW)
	{
		if (ctx->FocusedID == 0) {
			if (hovered && InputHelper::Pressed(VK_LBUTTON)) {
				ctx->FocusedID = GUI::Hash(name);
			}
		}
		else if (ctx->FocusedID == GUI::Hash(name)) {
			if (!InputHelper::Down(VK_LBUTTON)) {
				ctx->FocusedID = 0;

				if (hovered) {
					g_cfg[value].set<bool>(!g_cfg[value].get<bool>());
				}
			}
		}
	}
	
	PushCursorPos( CursorPos + vec2_t( 0, DrawSize.y + GUI::ObjectPadding( ) ) );

	return g_cfg[value].get<bool>();
}
