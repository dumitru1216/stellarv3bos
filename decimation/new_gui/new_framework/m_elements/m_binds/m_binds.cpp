#include "m_binds.h"

static short int StoredIndex = -1;
static bool m_State = false;
static int m_Key = -1;

std::string m_Name[ Gui::m_Maximum ];
static bool isGood = false;

char m_Buffer[ 128 ];
extern const char* m_Keys[ 254 ];

static short int StoredIndexList = -1;
static bool m_StateList = false;

static int m_Time;
static int m_Alpha[ Gui::m_Maximum ];

static int m_Animation[ Gui::m_Maximum ];
static int m_AnimationText[ Gui::m_Maximum ];

std::vector<std::string> m_Items = { "Always", "Hold on", "Toggle", "Hold off" };

Bind::Bind( int* m_Value, int* m_BindType )
{
	this->m_Value = m_Value;
	this->m_BindType = m_BindType;
}

void Bind::UpdateList( )
{
	auto TextSize = DrawList.GetTextSize( m_Name[ Gui::m_Control.GetIndex( ) ], Fonts::Main );

	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] == 100 && m_StateList && StoredIndexList == Gui::m_Control.GetIndex( ) && Gui::m_Input.KeyPressed( VK_LBUTTON )
		&& !Gui::m_Input.MousePointer( { m_Pos.x + 20, m_Pos.y + 4 }, { 90, ( float )( m_Items.size( ) * 18 ) } ) )
	{
		m_StateList = !m_StateList;
		StoredIndexList = -1;
	}

	if ( !Gui::m_Control.m_Activity[ 1 ] && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 2 ] && Gui::m_Control.IsPossible( ) && m_Animation[ Gui::m_Control.GetIndex( ) ] == 0 && m_Time == -1
		&& Gui::m_Input.MousePointer( { m_Pos.x - TextSize.x, m_Pos.y }, { 35, 10 } ) && Gui::m_Input.KeyPressed( VK_RBUTTON ) )
	{
		m_StateList = !m_StateList;
		StoredIndexList = Gui::m_Control.GetIndex( );
	}

	if ( m_Time >= 12 )
		m_Time = 12;

	else if ( m_Time <= 0 )
		m_Time = 0;

	if ( m_StateList )
		m_Time++;
	else
		m_Time--;

	Gui::m_Control.m_Opened[ Gui::m_Control.ControlType::BIND ] = m_Time;

	if ( StoredIndexList == Gui::m_Control.GetIndex( ) )
		m_FocusedList = m_StateList;

	Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::BIND ][ Gui::m_Control.GetIndex( ) ] = m_FocusedList;
}

void Bind::InitList( )
{
	if ( m_FocusedList )
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

	std::clamp<int>( m_Animation[ Gui::m_Control.GetIndex( ) ], 0, 100 );
	std::clamp<int>( m_AnimationText[ Gui::m_Control.GetIndex( ) ], 0, 255 );

	if ( m_Animation[ Gui::m_Control.GetIndex( ) ] != 0 )
	{
		for ( size_t i{ 0 }; i < m_Items.size( ); i++ )
		{
			if ( Gui::m_Input.KeyPressed( VK_LBUTTON ) && Gui::m_Input.MousePointer( { m_Pos.x + 21, m_Pos.y + 4 + ( i * 18 ) }, { 90, 15 } ) )
			{
				if ( m_Animation[ Gui::m_Control.GetIndex( ) ] == 100 )
				{
					*m_BindType = i;

					m_StateList = !m_StateList;
					StoredIndexList = -1;
				}
			}

			color_t m_Select = Gui::m_Input.MousePointer( { m_Pos.x + 21, m_Pos.y + 4 + ( i * 18 ) }, { 90, 15 } ) || *m_BindType == i ? color_t( 185, 185, 185, m_AnimationText[ Gui::m_Control.GetIndex( ) ] ) : color_t( 120, 120, 120, m_AnimationText[ Gui::m_Control.GetIndex( ) ] );
			Gui::m_External.TextDraw.insert( Gui::m_External.TextDraw.begin( ), Gui::External::EDrawText( m_Pos.x + 24, m_Pos.y + 4 + ( 18 * i ), m_Items[ i ], m_Select ) );
		}

		Gui::m_External.RectDraw.insert( Gui::m_External.RectDraw.begin( ), Gui::External::EDrawRect( m_Pos.x + 20 + 1, m_Pos.y + 1, 91 - 2, ( ( 18 * m_Items.size( ) ) + 2 ) * ( m_Animation[ Gui::m_Control.GetIndex( ) ] / 100.f ), color_t( 25, 20, 25 ) ) );
		Gui::m_External.RectDraw.insert( Gui::m_External.RectDraw.begin( ), Gui::External::EDrawRect( m_Pos.x + 20, m_Pos.y, 91, ( 18 * m_Items.size( ) + 4 ) * ( m_Animation[ Gui::m_Control.GetIndex( ) ] / 100.f ), color_t( 130, 125, 150 ) ) );
	}

	Gui::m_Control.m_Activity[ 3 ] = m_StateList;
}

void Bind::Update( )
{
	auto TextSize = DrawList.GetTextSize( m_Name[ Gui::m_Control.GetIndex( ) ], Fonts::Main );
	if ( !Gui::m_Control.color_tPickerOpened && Gui::m_Control.IsPossible( ) && GetAsyncKeyState( VK_LBUTTON ) && Gui::m_Input.MousePointer( { m_Pos.x - TextSize.x, m_Pos.y }, { 35, 10 } ) )
	{
		if ( !m_State )
			m_State = true;

		StoredIndex = Gui::m_Control.GetIndex( );
	}

	if ( StoredIndex == Gui::m_Control.GetIndex( ) )
		m_Focused = m_State;

	if ( m_Focused )
	{
		for ( int i = 0; i < 255; i++ )
		{
			if ( Gui::m_Input.KeyPressed( i ) )
			{
				if ( i == VK_ESCAPE )
				{
					*m_Value = -1;
					StoredIndex = -1;
					return;
				}

				*m_Value = i;
				StoredIndex = -1;
				return;
			}
		}
	}

	std::string Container = "[";
	std::string NextContainer = "]";

	if ( m_Focused ) {
		m_Name[ Gui::m_Control.GetIndex( ) ] = "[-]";
	}
	else
	{
		if ( *m_Value >= 0 ) {
			m_Name[ Gui::m_Control.GetIndex( ) ] = Container + m_Keys[ *m_Value ] + NextContainer;

			if ( m_Name[ Gui::m_Control.GetIndex( ) ].c_str( ) ) {
				isGood = true;
			}
			else
			{
				if ( GetKeyNameText( *m_Value << 16, m_Buffer, 127 ) )
				{
					m_Name[ Gui::m_Control.GetIndex( ) ] = m_Buffer;
					isGood = true;
				}
			}
		}

		if ( !isGood ) {
			m_Name[ Gui::m_Control.GetIndex( ) ] = "[-]";
		}
	}
}

void Bind::Draw( )
{
	if ( m_Name[ Gui::m_Control.GetIndex( ) ] == "" )
		m_Name[ Gui::m_Control.GetIndex( ) ] = "[-]";

	auto TextSize = DrawList.GetTextSize( m_Name[ Gui::m_Control.GetIndex( ) ], Fonts::Main );
	DrawList.DrawString( { m_Pos.x - TextSize.x, m_Pos.y }, m_Name[ Gui::m_Control.GetIndex( ) ], color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
}

const char* m_Keys[ 254 ] = {
	"invld", "m1", "m2", "brk", "m3", "m4", "m5",
	"invld", "bspc", "tab", "invld", "invld", "invld", "enter", "invld", "invld", "shi",
	"ctrl", "alt", "pau", "caps", "invld", "invld", "invld", "invld", "invld", "invld",
	"esc", "invld", "invld", "invld", "invld", "space", "pgup", "pgdown", "end", "home", "left",
	"up", "right", "down", "invld", "prnt", "invld", "prtscr", "ins", "del", "invld", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
	"v", "w", "x", "y", "z", "lftwin", "rghtwin", "invld", "invld", "invld", "num0", "num1",
	"num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3",
	"f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20",
	"f21",
	"f22", "f23", "f24", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"num lock", "scroll lock", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "lshft", "rshft", "lctrl",
	"rctrl", "lmenu", "rmenu", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "ntrk", "ptrk", "stop", "play", "invld", "invld",
	"invld", "invld", "invld", "invld", ";", "+", ",", "-", ".", "/?", "~", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "{", "\\|", "}", "'\"", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld", "invld",
	"invld", "invld"
};