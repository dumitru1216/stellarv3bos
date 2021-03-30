#include "m_listbox.h"
#include <cctype>

static int m_ScrollPos[ Gui::m_Maximum ];
static bool m_ScrollState[ Gui::m_Maximum ];

static int m_Alpha[ Gui::m_Maximum ];

ListBox::ListBox( std::string m_Name, int* m_Select, std::vector<std::string>& m_Items )
{
	this->m_Name = m_Name;
	this->m_Select = m_Select;
	this->m_Items = &m_Items;
}

ListBox::ListBox( std::string m_Name, int* m_Select, std::vector<std::string>& m_Items, std::string& m_Filter )
{
	this->m_Name = m_Name;
	this->m_Select = m_Select;
	this->m_Items = &m_Items;
	this->m_Filter = &m_Filter;
}

void ListBox::Update( )
{
	GetItems( );
}

void ListBox::Scroll( int m_Limit )
{
	static size_t m_Lost[ Gui::m_Maximum ];
	if ( m_Lost[ Gui::m_Control.GetIndex( ) ] != m_Temp.size( ) ) {
		m_ScrollPos[ Gui::m_Control.GetIndex( ) ] = 0;
		m_Lost[ Gui::m_Control.GetIndex( ) ] = m_Temp.size( );
	}

	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x + 6, m_Pos.y + 18 },
		{ 120, 245 }
	);

	if ( !m_ScrollState[ Gui::m_Control.GetIndex( ) ] && isHovered && m_Temp.size( ) > 12 )
	{
		float ratio = 13 / float( m_Temp.size( ) );
		float some_shit = m_Pos.y;

		float size_ratio = 13 / float( m_Temp.size( ) );
		size_ratio *= 120;

		float height_delta = some_shit + size_ratio - ( 120 );
		if ( height_delta > 0 )
			some_shit -= height_delta;

		float pos_ratio = float( some_shit ) / float( 120 );
		float m_prikol = pos_ratio * m_Temp.size( );

		if ( Gui::m_Input.GetMouseWheel( ) != 0 && isHovered )
		{
			m_ScrollPos[ Gui::m_Control.GetIndex( ) ] += Gui::m_Input.GetMouseWheel( ) * ( -1 );
			Gui::m_Input.SetMouseWheel( 0 );

			if ( m_ScrollPos[ Gui::m_Control.GetIndex( ) ] < 0 )
				m_ScrollPos[ Gui::m_Control.GetIndex( ) ] = 0;

			if ( m_ScrollPos[ Gui::m_Control.GetIndex( ) ] > ( int )m_prikol )
				m_ScrollPos[ Gui::m_Control.GetIndex( ) ] = ( int )m_prikol;
		}
	}
}

void ListBox::Draw( )
{
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 240, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 245, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 245, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ 185, 245 }
	);

	if ( isHovered && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ]
		&& !Gui::m_Control.m_Activity[ 3 ] )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] += 10;
	else
		m_Alpha[ Gui::m_Control.GetIndex( ) ] -= 10;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] > 155 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 155;

	if ( m_Alpha[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_Alpha[ Gui::m_Control.GetIndex( ) ] = 0;

	std::clamp<int>( m_Alpha[ Gui::m_Control.GetIndex( ) ], 0, 155 );

	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 245, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );

#pragma region list

	DrawList.DrawString( { m_Pos.x + 30, m_Pos.y + 4 }, m_Name, color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
	DrawList.Rect( m_Pos.x + 27, m_Pos.y + 16, 130, 215, color_t( 130, 125, 150 ) );

	if ( !m_Temp.empty( ) )
	{
		int DrawnItems = 0;
		int ItemsToDraw = 13;

		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		DrawList.SetViewport( { ( DWORD )m_Pos.x + 6, ( DWORD )m_Pos.y + 18, ( DWORD )151, ( DWORD )210, 0.f, 1.f } );

		for ( int i{ m_ScrollPos[ Gui::m_Control.GetIndex( ) ] }; ( i < m_Temp.size( ) && DrawnItems < ItemsToDraw ); i++ )
		{
			bool isHovered = Gui::m_Input.MousePointer
			(
				{ m_Pos.x + 34, ( m_Pos.y + 19 + DrawnItems * 16 ) },
				{ 120, 15 }
			);

			if ( isHovered && Gui::m_Input.KeyPressed( VK_LBUTTON )
				&& Gui::m_Control.IsPossible( ) && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.color_tPickerOpened )
				*m_Select = m_Temp[ i ].second;

			color_t m_Color = isHovered && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 2 ]
				&& !Gui::m_Control.m_Activity[ 3 ] || *m_Select == m_Temp[ i ].second ? color_t( 185, 185, 185 ) : color_t( 120, 120, 120 );

			DrawList.DrawString( { m_Pos.x + 34, ( m_Pos.y + 20 + DrawnItems * 16 ) }, m_Temp[ i ].first, m_Color, Fonts::Main, font_flags::drop_shadow );

			DrawnItems++;
		}

		DrawList.SetViewport( m_Old );
		Scroll( 0 );
	}
}

std::string ListBox::ToLower( std::string String )
{
	std::transform( String.begin( ), String.end( ), String.begin( ), ( int( * )( int ) )::tolower );
	return String;
}

void ListBox::GetItems( )
{
	if ( m_LowerItems.empty( ) || m_LowerItems.size( ) != m_Items->size( ) )
	{
		m_LowerItems.clear( );
		for ( int i = 0; i < m_Items->size( ); i++ )
		{
			std::string temp_s = ( *m_Items )[ i ];
			m_LowerItems.push_back( temp_s );
		}
	}

	if ( !m_Filter && m_Temp.empty( ) || m_Temp.size( ) != m_Items->size( ) && ( !m_Filter || m_Filter->empty( ) ) )
	{
		m_Temp.clear( );
		for ( int i = 0; i < m_Items->size( ); i++ ) {
			m_Temp.emplace_back( ( *m_Items )[ i ], i );
		}
	}

	if ( !m_Filter )
		return;

	if ( m_PreviousFilter == *m_Filter )
		return;

	m_PreviousFilter = *m_Filter;
	const auto l_filter = ToLower( *m_Filter );

	m_Temp.clear( );

	for ( int i = 0; i < m_Items->size( ); i++ )
	{
		if ( m_Filter != nullptr )
		{
			std::string temp_s = m_LowerItems[ i ];
			if ( !( temp_s.find( l_filter ) != std::string::npos ) )
				continue;
		}

		m_Temp.emplace_back( m_Items->at( i ), i );
	}
}