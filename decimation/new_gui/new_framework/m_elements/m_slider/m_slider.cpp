#include "m_slider.h"

/* animation related */
static int m_Alpha[ Gui::m_Maximum ];
static int m_AlphaText[ Gui::m_Maximum ];

/* setup slider */
Slider::Slider( std::string m_Name, float* m_Value, int m_Min, int m_Max, std::string m_Suffix ) {
	/* values */
	this->m_Name = m_Name;
	this->m_Value = m_Value;
	this->m_Min = m_Min;
	this->m_Max = m_Max;
	this->m_Suffix = m_Suffix;
}

/* update slider before drawing this */
void Slider::Update( ) {
	static auto StoredIndex = -1;
	auto Delta = m_Max - m_Min;

	if ( Gui::m_Control.IsPossible( ) && !Gui::m_Control.color_tPickerOpened ) {
		if ( Gui::m_Input.KeyPressed( VK_LBUTTON ) & 1 && Gui::m_Input.MousePointer( vec2_t( m_Pos.x + 28, m_Pos.y + 16 ), vec2_t( 130, 6 ) ) )
			StoredIndex = Gui::m_Control.GetIndex( );
	}

	if ( GetAsyncKeyState( VK_LBUTTON ) && StoredIndex == Gui::m_Control.GetIndex( ) )
	{
		*m_Value = m_Min + Delta * ( Gui::m_Input.GetMousePos( ).x - m_Pos.x - 26 ) / 130;

		if ( *m_Value < m_Min )
			*m_Value = m_Min;

		if ( *m_Value > m_Max )
			*m_Value = m_Max;
	}

	if ( !GetAsyncKeyState( VK_LBUTTON ) )
		StoredIndex = -1;
}

/* draw slider */
void Slider::Draw( ) {
	/* rendering related */
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 24, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );
	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 30, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 30, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	/* animation related */
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ 185, 30 }
	);

	if ( isHovered && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ] )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] += 5;
	else
		m_Alpha[ Gui::m_Control.GetIndex( ) ] -= 5;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] > 155 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 155;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 0;

	/* clamp animation */
	std::clamp<int>( m_Alpha[ Gui::m_Control.GetIndex( ) ], 0, 155 );

	/* rendering related */
	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 30, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );

#pragma region slider

	/* values */
	auto m_MinDelta = *m_Value - m_Min;
	auto Delta = m_Max - m_Min;
	auto m_Total = ( m_MinDelta / Delta ) * 130;

	/* animation related */
	if ( isHovered && !Gui::m_Control.m_Activity[ 0 ] )
		m_AlphaText[ Gui::m_Control.GetIndex( ) ] += 10;
	else
		m_AlphaText[ Gui::m_Control.GetIndex( ) ] -= 10;

	if ( m_AlphaText[ Gui::m_Control.GetIndex( ) ] > 255 )
		m_AlphaText[ Gui::m_Control.GetIndex( ) ] = 255;

	if ( m_AlphaText[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_AlphaText[ Gui::m_Control.GetIndex( ) ] = 0;

	/* clamp animation */
	std::clamp<int>( m_AlphaText[ Gui::m_Control.GetIndex( ) ], 0, 255 );

	/* rendering related */
	DrawList.FilledRect( m_Pos.x + 29, m_Pos.y + 17, 130, 5, color_t( 45, 45, 50, 255 ) );
	DrawList.FilledRect( m_Pos.x + 29, m_Pos.y + 17, m_Total, 5, color_t( 130, 130, 170, 255 ) );
	DrawList.DrawString( { m_Pos.x + 30, m_Pos.y + 4 }, m_Name, color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );

	/* get slider value */
	std::string m_Buf = std::to_string( ( int )*m_Value );
	m_Buf.append( "%" );
	auto m_Size = DrawList.GetTextSize( m_Buf, Fonts::Main );

	/* rendering related */
	DrawList.DrawString( { m_Pos.x + 160 - m_Size.x, m_Pos.y + 4 }, m_Buf, color_t( 255, 255, 255, m_AlphaText[ Gui::m_Control.GetIndex( ) ] ), Fonts::Main, font_flags::drop_shadow );
}