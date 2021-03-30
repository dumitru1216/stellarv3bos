#include "m_textinput.h"

extern const char* LowerCase[ 254 ];
extern const char* UpperCase[ 254 ];

static short int StoredIndex = -1;
static bool m_State = false;

float m_Blink;
static int m_Alpha[ Gui::m_Maximum ];

TextInput::TextInput( std::string* m_Label )
{
	this->m_Label = m_Label;
}

void TextInput::Update( )
{
	if ( m_State && StoredIndex == Gui::m_Control.GetIndex( ) && Gui::m_Input.KeyPressed( VK_LBUTTON ) )
	{
		m_State = !m_State;
		StoredIndex = -1;
	}

	if ( !Gui::m_Control.color_tPickerOpened && Gui::m_Control.IsPossible( ) && Gui::m_Input.KeyPressed( VK_LBUTTON )
		&& Gui::m_Input.MousePointer( { m_Pos.x + 7, m_Pos.y + 6 }, { 170, 18 } ) )
	{
		m_State = !m_State;
		StoredIndex = Gui::m_Control.GetIndex( );
	}

	if ( StoredIndex == Gui::m_Control.GetIndex( ) )
		m_Focused = m_State;

	Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::TEXTBOX ][ Gui::m_Control.GetIndex( ) ] = m_Focused;

	if ( m_Focused )
	{
		std::string m_Str = *m_Label;

		for ( int i = 0; i < 255; i++ ) {
			if ( Gui::m_Input.KeyPressed( i ) ) {
				if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT ) {
					StoredIndex = -1;
					return;
				}

				if ( strlen( m_Str.c_str( ) ) != 0 ) {
					if ( GetAsyncKeyState( VK_BACK ) ) {
						*m_Label = m_Str.substr( 0, strlen( m_Str.c_str( ) ) - 1 );
					}
				}

				if ( strlen( m_Str.c_str( ) ) < 27 && i != NULL && UpperCase[ i ] != nullptr ) {
					if ( GetAsyncKeyState( VK_SHIFT ) ) {
						*m_Label = m_Str + UpperCase[ i ];
					}
					else {
						*m_Label = m_Str + LowerCase[ i ];
					}

					return;
				}

				if ( strlen( m_Str.c_str( ) ) < 27 && i == 32 ) {
					*m_Label = m_Str + " ";
					return;
				}
			}
		}
	}
}

void TextInput::Draw( )
{
	DrawList.GradientVertical( m_Pos.x, m_Pos.y, 185, 6, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientVertical( m_Pos.x, m_Pos.y + 24, 185, 6, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

	DrawList.GradientHorizontal( m_Pos.x, m_Pos.y, 6, 30, color_t( 100, 100, 100, 100 ), color_t( 45, 45, 45, 0 ) );
	DrawList.GradientHorizontal( m_Pos.x + 179, m_Pos.y, 6, 30, color_t( 45, 45, 45, 0 ), color_t( 100, 100, 100, 100 ) );

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

	DrawList.Rect( m_Pos.x, m_Pos.y, 185, 30, color_t( 130, 125, 150, 100 + m_Alpha[ Gui::m_Control.GetIndex( ) ] ) );

#pragma region text_input

	std::string ToDraw = *m_Label;

	if ( GetTickCount64( ) >= m_Blink )
		m_Blink = GetTickCount64( ) + 800;

	if ( m_Focused && GetTickCount64( ) > ( m_Blink - 400 ) )
		ToDraw += "_";

	DrawList.Rect( m_Pos.x + 7, m_Pos.y + 6, 170, 18, color_t( 130, 125, 150 ) );
	DrawList.DrawString( { m_Pos.x + 11, m_Pos.y + 9 }, ToDraw, color_t( 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
}

const char* LowerCase[ 254 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

const char* UpperCase[ 254 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "_", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };