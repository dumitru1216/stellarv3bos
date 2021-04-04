#include "m_checkbox.h"

static int m_Alpha[ Gui::m_Maximum ];
static int m_Progress[ Gui::m_Maximum ];

Checkbox::Checkbox( std::string m_Name, bool* m_Value )
{
	this->m_Name = m_Name;
	this->m_Value = m_Value;
}

void Checkbox::Update( )
{
	auto m_Size = DrawList.GetTextSize( m_Name, Fonts::Main );

	if ( Gui::m_Control.IsPossible( ) && !Gui::m_Control.color_tPickerOpened && !Gui::m_Control.m_Activity[ 0 ] ) {
		if ( Gui::m_Input.MousePointer( { m_Pos.x, m_Pos.y }, { 135, 20 } ) && Gui::m_Input.KeyPressed( VK_LBUTTON ) )
			*this->m_Value = !*this->m_Value;
	}
}

void Checkbox::Draw( )
{
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 14, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 20, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 20, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ 185, 20 }
	);

	if ( isHovered && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ] && !Gui::m_Control.m_Activity[ 3 ] )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] += 5;
	else
		m_Alpha[ Gui::m_Control.GetIndex( ) ] -= 5;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] > 155 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 155;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 0;

	std::clamp<int>( m_Alpha[ Gui::m_Control.GetIndex( ) ], 0, 155 );

	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 20, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );

#pragma region check

	if ( *m_Value )
		m_Progress[ Gui::m_Control.GetIndex( ) ] += 5;

	else
		m_Progress[ Gui::m_Control.GetIndex( ) ] -= 5;

	if ( m_Progress[ Gui::m_Control.GetIndex( ) ] > 100 )
		m_Progress[ Gui::m_Control.GetIndex( ) ] = 100;

	if ( m_Progress[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Progress[ Gui::m_Control.GetIndex( ) ] = 0;

	std::clamp<int>( m_Progress[ Gui::m_Control.GetIndex( ) ], 0, 100 );

	if ( !g_loser.menu.menu_color_1_ch ) {
		DrawList.GradientVertical( m_Pos.x + 7, m_Pos.y + 5, 10 * ( m_Progress[ Gui::m_Control.GetIndex( ) ] / 100.f ),
			10 * ( m_Progress[ Gui::m_Control.GetIndex( ) ] / 100.f ), color_t( 140, 130, 160 ), color_t( 130, 115, 150 ) );
	}
	else {
		DrawList.GradientVertical( m_Pos.x + 7, m_Pos.y + 5, 10 * ( m_Progress[ Gui::m_Control.GetIndex( ) ] / 100.f ),
			10 * ( m_Progress[ Gui::m_Control.GetIndex( ) ] / 100.f ), g_loser.menu.menu_color_1, g_loser.menu.menu_color_1 );
	}

	DrawList.Rect( m_Pos.x + 7, m_Pos.y + 5, 10, 10, color_t( 130, 125, 150 ) );
	DrawList.DrawString( { m_Pos.x + 30, m_Pos.y + 4 }, m_Name, color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
}