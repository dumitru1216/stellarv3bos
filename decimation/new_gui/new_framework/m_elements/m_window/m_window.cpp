#include "m_window.h"

/* window */
Window::Window( std::string m_Name, vec2_t* m_Pos, vec2_t m_Size, std::vector<std::string> m_WindowTabs, int* m_Selected ) {
	/* window setup related */
	this->m_Name = m_Name;
	this->m_WindowPosition = m_Pos;
	this->m_Size = m_Size;

	/* tab setup related */
	this->m_WindowTabs = m_WindowTabs;
	this->m_Selected = m_Selected;

	/* run setup */
	this->Run( );
}

/* run window elements */
void Window::Run( ) {
	/* elements */
	InitPosition( );
	Draw( );
	InitTabs( );
}

/* draw window */
void Window::Draw( ) {
	/* animation related */
	static int m_Alpha;
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ this->m_WindowPosition->x, this->m_WindowPosition->y },
		{ this->m_Size.x, this->m_Size.y }
	);

	if ( isHovered )
		m_Alpha += 5;
	else
		m_Alpha -= 5;

	if ( m_Alpha > 130 )
		m_Alpha = 130;

	if ( m_Alpha < 0 )
		m_Alpha = 0;

	std::clamp<int>( m_Alpha, 0, 130 );

	/* menu related */
	DrawList.FilledRect( this->m_WindowPosition->x, this->m_WindowPosition->y, this->m_Size.x, this->m_Size.y, color_t( 20, 15, 20 ) );
	DrawList.GradientVertical( this->m_WindowPosition->x, this->m_WindowPosition->y, this->m_Size.x, 25, color_t( 35, 30, 35 ), color_t( 45, 45, 60 ) );
	DrawList.GradientVertical( this->m_WindowPosition->x, this->m_WindowPosition->y + 25, this->m_Size.x, 10, color_t( 100, 100, 100, 50 ), color_t( 45, 45, 45, 0 ) );
	
	if ( !g_loser.menu.menu_color_1_ch ) {
		DrawList.FilledRect( this->m_WindowPosition->x, this->m_WindowPosition->y + 25, this->m_Size.x, 1, color_t( 130, 125, 150 ) );
	}
	else {
		DrawList.FilledRect( this->m_WindowPosition->x, this->m_WindowPosition->y + 25, this->m_Size.x, 1, g_loser.menu.menu_color_1 );
	}

	DrawList.Rect( this->m_WindowPosition->x, this->m_WindowPosition->y, this->m_Size.x, this->m_Size.y, color_t( 80, 80, 100, 125 + m_Alpha ) );
	DrawList.DrawString( { this->m_WindowPosition->x + 10, this->m_WindowPosition->y + 4 }, this->m_Name, color_t( 255, 255, 255, 255 ), Fonts::Icon, font_flags::drop_shadow );
}

/* initialize menu position */
void Window::InitPosition( ) {
	/* drag related */
	static bool m_Drag{ false };
	static vec2_t m_Offset{ };

	/* position related */
	vec2_t m_Delta = Gui::m_Input.GetMousePos( ) - m_Offset;

	if ( m_Drag && !GetAsyncKeyState( VK_LBUTTON ) )
		m_Drag = false;

	if ( m_Drag && GetAsyncKeyState( VK_LBUTTON ) )
		*this->m_WindowPosition = m_Delta;

	if ( Gui::m_Input.MousePointer( vec2_t( this->m_WindowPosition->x, this->m_WindowPosition->y ), vec2_t( 210, 20 ) ) ) {
		m_Drag = true;
		m_Offset = Gui::m_Input.GetMousePos( ) - *this->m_WindowPosition;
	}

	/* window position related */
	if ( this->m_WindowPosition->x < 0 )
		this->m_WindowPosition->x = 0;

	if ( this->m_WindowPosition->y < 0 )
		this->m_WindowPosition->y = 0;
}

/* initialize tabs */
void Window::InitTabs( ) {
	/* check tabs open close and hovered */
	if ( this->m_WindowTabs.empty( ) )
		return;

	/* check opened too */
	bool isCan = !Gui::m_Control.color_tPickerOpened &&
		!Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ] && !Gui::m_Control.m_Activity[ 3 ]
		&& Gui::m_Control.IsPossible( );

	/* setup alpha */
	static int m_Alpha[ 6 ];
	static int isSwitched{ 0 };
	static int m_AlphaNext{ 255 };
	static int m_Change;

	/* fix alpha */
	if ( isSwitched != *this->m_Selected )
	{
		m_AlphaNext = -1;
		isSwitched = *this->m_Selected;
	}

	if ( m_AlphaNext != 255 )
		m_AlphaNext += 5;

	if ( m_AlphaNext > 255 )
		m_AlphaNext = 255;

	if ( m_AlphaNext < 0 )
		m_AlphaNext = 0;

	std::clamp<int>( m_AlphaNext, 0, 255 );

	/* tab struct */
	for ( int i = 0; i < this->m_WindowTabs.size( ); i++ )
	{
		if ( i == 3 )
			m_Change = 3;
		else
			m_Change = 0;

		if ( isCan && Gui::m_Input.MousePointer( { this->m_WindowPosition->x + 205 - m_Change + ( i * 55 ),
			this->m_WindowPosition->y + 6 }, { 50, 10 } ) )
		{
			if ( Gui::m_Input.KeyPressed( VK_LBUTTON ) )
				*this->m_Selected = i;

			m_Alpha[ i ] += 5;
		}
		else
			m_Alpha[ i ] -= 5;

		if ( m_Alpha[ i ] > 100 )
			m_Alpha[ i ] = 100;

		if ( m_Alpha[ i ] < 0 )
			m_Alpha[ i ] = 0;

		std::clamp<int>( m_Alpha[ i ], 0, 100 );

		color_t clr = i == *this->m_Selected ? color_t( 215, 215, 215 ) : color_t( 120, 120, 120, 255 - m_Alpha[ i ] );

		auto m_Size = DrawList.GetTextSize( this->m_WindowTabs[ i ].c_str( ), Fonts::Main );

		if ( i == *this->m_Selected )
		{
			DrawList.FilledRect( this->m_WindowPosition->x + 215 - m_Change + ( i * 55 ),
				this->m_WindowPosition->y + 19, m_Size.x, 1, color_t( 130, 130, 150, m_AlphaNext ) );

			DrawList.GradientVertical( this->m_WindowPosition->x + 215 - m_Change + ( i * 55 ), this->m_WindowPosition->y + 9,
				m_Size.x, 10, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 50 ) );
		}

		DrawList.DrawString( { this->m_WindowPosition->x + 215 - m_Change + ( i * 55 ), this->m_WindowPosition->y + ( i == *this->m_Selected ? 4 : 6 ) },
			this->m_WindowTabs[ i ].c_str( ), clr, Fonts::Main, font_flags::drop_shadow );
	}
}
void Window::AddGroup( Groupbox* Groupbox )
{
	Groupbox->SetVisible( Groupbox->m_Tab == *this->m_Selected );

	if ( Groupbox->GetVisible( ) )
	{
		Groupbox->SetPos( *this->m_WindowPosition );
		Groupbox->Run( );
	}
}