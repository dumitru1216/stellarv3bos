#include "m_combo.h"

/* values */
static auto StoredIndex = -1;
static bool m_State = false;

/* animation related */
static int m_Time;
static int m_Alpha[ Gui::m_Maximum ];
static int m_Animation[ Gui::m_Maximum ];
static int m_AnimationText[ Gui::m_Maximum ];

/* combo related setuping */
Combo::Combo( std::string m_Name, int* m_Value, std::vector<std::string> m_Items ) {
	/* values */
	this->m_Name = m_Name;
	this->m_Value = m_Value;
	this->m_Items = m_Items;
}

/* updating combobox before drawing first box */
void Combo::Update( ) {
	/* animation related */
	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] == 100 && m_State && StoredIndex == Gui::m_Control.GetIndex( ) && Gui::m_Input.KeyPressed( VK_LBUTTON )
		&& !Gui::m_Input.MousePointer( { m_Pos.x + 31, m_Pos.y + 40 }, { 130, ( float )( m_Items.size( ) * 18 ) } ) )
	{
		m_State = !m_State;
		StoredIndex = -1;
	}

	/* checking and updating when colorpicker is opened */
	if ( !Gui::m_Control.m_Activity[ 0 ] && m_Animation[ Gui::m_Control.GetIndex( ) ] == 0 && !Gui::m_Control.color_tPickerOpened && Gui::m_Control.m_Opened[ Gui::m_Control.ControlType::MULTICOMBO ] == -1 && m_Time == -1 && Gui::m_Input.KeyPressed( VK_LBUTTON )
		&& Gui::m_Input.MousePointer( { m_Pos.x + 27, m_Pos.y + 16 }, { 130, 18 } ) && !Gui::m_Control.m_Activity[ 3 ] )
	{
		m_State = !m_State;
		StoredIndex = Gui::m_Control.GetIndex( );
	}

	/* animation and time related */
	if ( m_Time >= 12 )
		m_Time = 12;

	else if ( m_Time <= 0 )
		m_Time = 0;

	if ( m_State )
		m_Time++;
	else
		m_Time--;

	/* checking controls */
	Gui::m_Control.m_Opened[ Gui::m_Control.ControlType::COMBO ] = m_Time;

	if ( StoredIndex == Gui::m_Control.GetIndex( ) )
		m_Focused = m_State;

	Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::COMBO ][ Gui::m_Control.GetIndex( ) ] = m_Focused;
}

/* drawing first box */
void Combo::DrawBox( ) {
	/* drawing related */
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 34, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );
	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 40, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 40, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	/* aniamtion related */
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ 185, 40 }
	);

	if ( isHovered && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ] && !Gui::m_Control.m_Activity[ 3 ] )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] += 10;
	else
		m_Alpha[ Gui::m_Control.GetIndex( ) ] -= 10;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] > 155 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 155;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 0;

	/* clamp animation */
	std::clamp<int>( m_Alpha[ Gui::m_Control.GetIndex( ) ], 0, 155 );

	/* drawing related */
	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 40, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );
}

/* drawing 2nd box ( fulll combo ) */
void Combo::Draw( ){
#pragma region combo
	/* arrow drawing */
	auto Arrow = [ ]( float x, float y, bool turn ) {
		if ( turn )
		{
			for ( float i = 10; i >= 4; --i ) {
				float offset = 10 - i;
				DrawList.Line( x + offset, y + offset, x + offset + std::clamp( i - offset, ( float )0, ( float )10 ), y + offset, color_t( 170, 170, 170 ) );
			}
		}
		else
		{
			DrawList.Line( x + 4, y, x + 6, y, color_t( 170, 170, 170 ) );
			DrawList.Line( x + 3, y + 1, x + 7, y + 1, color_t( 170, 170, 170 ) );
			DrawList.Line( x + 2, y + 2, x + 8, y + 2, color_t( 170, 170, 170 ) );
			DrawList.Line( x + 1, y + 3, x + 9, y + 3, color_t( 170, 170, 170 ) );
			DrawList.Line( x, y + 4, x + 10, y + 4, color_t( 170, 170, 170 ) );
		}
	};

	/* drawing related */
	DrawList.DrawString( { m_Pos.x + 30, m_Pos.y + 4 }, m_Name, color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
	DrawList.Rect( m_Pos.x + 27, m_Pos.y + 16, 130, 18, color_t( 130, 125, 150 ) );
	DrawList.DrawString( { m_Pos.x + 31, m_Pos.y + 19 }, m_Items[ *m_Value ], color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );

	Arrow( m_Pos.x + 140, m_Pos.y + 23, !m_Focused );

	/* animation related */
	if ( m_Focused )
	{
		m_Animation[ Gui::m_Control.GetIndex( ) ] += 10;
		m_AnimationText[ Gui::m_Control.GetIndex( ) ] += 15;
	}
	else
	{
		m_Animation[ Gui::m_Control.GetIndex( ) ] -= 10;
		m_AnimationText[ Gui::m_Control.GetIndex( ) ] -= 15;
	}

	if ( m_AnimationText[ Gui::m_Control.GetIndex( ) ] > 255 )
		m_AnimationText[ Gui::m_Control.GetIndex( ) ] = 255;

	if ( m_AnimationText[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_AnimationText[ Gui::m_Control.GetIndex( ) ] = 0;

	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] > 100 )
		m_Animation[ Gui::m_Control.GetIndex( ) ] = 100;

	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Animation[ Gui::m_Control.GetIndex( ) ] = 0;

	/* clamp animation */
	std::clamp<int>( m_Animation[ Gui::m_Control.GetIndex( ) ], 0, 100 );
	std::clamp<int>( m_AnimationText[ Gui::m_Control.GetIndex( ) ], 0, 255 );

	/* fix animation */
	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] != 0 )
	{
		for ( size_t i{ 0 }; i < m_Items.size( ); i++ )
		{
			if ( Gui::m_Input.KeyPressed( VK_LBUTTON ) && Gui::m_Input.MousePointer( { m_Pos.x + 31, m_Pos.y + 41 + ( i * 18 ) }, { 130, 15 } ) )
			{
				if ( m_Animation[ Gui::m_Control.GetIndex( ) ] == 100 )
				{
					*m_Value = i;

					m_State = !m_State;
					StoredIndex = -1;
				}
			}

			color_t m_Select = Gui::m_Input.MousePointer( { m_Pos.x + 31, m_Pos.y + 41 + ( i * 18 ) }, { 130, 15 } ) || *m_Value == i ? color_t( 185, 185, 185, m_AnimationText[ Gui::m_Control.GetIndex( ) ] ) : color_t( 120, 120, 120, m_AnimationText[ Gui::m_Control.GetIndex( ) ] );
			Gui::m_External.TextDraw.insert( Gui::m_External.TextDraw.begin( ), Gui::External::EDrawText( m_Pos.x + 31, m_Pos.y + 42 + ( 18 * i ), m_Items[ i ], m_Select ) );
		}

		Gui::m_External.RectDraw.insert( Gui::m_External.RectDraw.begin( ), Gui::External::EDrawRect( m_Pos.x + 27 + 1, m_Pos.y + 39, 131 - 2, ( ( 18 * m_Items.size( ) ) + 2 ) * ( m_Animation[ Gui::m_Control.GetIndex( ) ] / 100.f ), color_t( 25, 20, 25 ) ) );
		Gui::m_External.RectDraw.insert( Gui::m_External.RectDraw.begin( ), Gui::External::EDrawRect( m_Pos.x + 27, m_Pos.y + 38, 131, ( 18 * m_Items.size( ) + 4 ) * ( m_Animation[ Gui::m_Control.GetIndex( ) ] / 100.f ), color_t( 130, 125, 150 ) ) );
	}

	Gui::m_Control.m_Activity[ 1 ] = m_State;
}