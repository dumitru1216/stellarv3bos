#include "m_button.h"

/* vars */
static short int StoredIndex = -1;
static bool m_State = false;
static int m_Alpha[ Gui::m_Maximum ];

/* button setup */
Button::Button( std::string m_Name, std::function< void( ) > Function ) {
	/* setup vars */
	this->m_Name = m_Name;
	this->Function = Function;
}

/* update buton */
void Button::Update( ) {
	/* update key */
	if ( !Gui::m_Input.KeyPressed( VK_LBUTTON ) && Gui::m_Input.MousePointer( { m_Pos.x + 27, m_Pos.y + 6 }, { 130, 18 } ) )
	{
		if ( m_State )
			Function( );

		m_State = false;
	}

	/* check colorpicker */
	if ( !Gui::m_Control.color_tPickerOpened && Gui::m_Control.IsPossible( ) && Gui::m_Input.KeyPressed( VK_LBUTTON ) && Gui::m_Input.MousePointer( { m_Pos.x + 27, m_Pos.y + 6 }, { 130, 18 } ) )
	{
		m_State = true;
		StoredIndex = Gui::m_Control.GetIndex( );
	}

	if ( StoredIndex == Gui::m_Control.GetIndex( ) )
		m_Focused = m_State;

}

/* draw button */
void Button::Draw( ) {
	/* draw this */
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 24, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 30, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 30, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	/* check hovered */
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ 185, 30 }
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

	/* setup */
	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 30, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );

#pragma region button

	DrawList.Rect( m_Pos.x + 27, m_Pos.y + 6, 130, 18, color_t( 130, 125, 150 ) );
	DrawList.DrawString( { m_Pos.x + ( 180 / 2 ), m_Pos.y + 13 }, m_Name, color_t( 255, 255, 255 ), Fonts::Main, font_flags::centered_x | font_flags::centered_y | font_flags::drop_shadow );

}