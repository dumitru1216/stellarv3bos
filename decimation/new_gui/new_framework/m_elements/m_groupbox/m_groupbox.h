#pragma once
#include "../m_window/m_window.h"

/* dimanic class */
class Checkbox;
class Slider;
class Combo;
class Multi;
class ColorPicker;
class Bind;
class TextInput;
class Button;
class ListBox;

class Groupbox : public Base {
public:

	// Some groupbox flags.
	enum Flags
	{
		GROUP_NO_DRAW = 1 << 0,
		GROUP_NO_DRAW_TEXT = 1 << 1
	};

	// Constructor.
	Groupbox( std::string m_Name, int m_GroupNum, float X, float  Y, float  m_Width, float  m_Height, int m_CurrentTab = 0, uint32_t m_Flags = 0 );

	void Run( );
	void Draw( );

	// Functions.
	void SetVisible( bool m_Vis );
	bool GetVisible( );

	void SetPos( vec2_t m_Pos );
	vec2_t GetPos( );

	bool m_Visible;

	std::string m_Name;
	uint32_t m_Flags;

	vec2_t m_Size;
	vec2_t m_Pos;

	int m_Tab;
	int m_GroupNum;

	void SetLimit( );
	bool IsHovered( );

	struct
	{

		float m_OffsetX;
		float m_OffsetY[ Gui::m_Maximum ];

	} m_Elements;

public:

	void AddElement( Checkbox* m_Check );
	void AddElement( Slider* m_Slider );
	void AddElement( Combo* m_Combo );
	void AddElement( Multi* m_Multi );
	void AddElement( ColorPicker* m_Color );
	void AddElement( Bind* m_Bind );
	void AddElement( TextInput* m_Input );
	void AddElement( Button* m_Button );
	void AddElement( ListBox* m_List );

	void AddEmpty( int m_EmptyPlace, int m_Slide );
	void InitScroll( );
};