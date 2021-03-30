#pragma once
#include "../m_groupbox/m_groupbox.h"

/* main class of colorpicker */
class ColorPicker : public Base {
public:
	/* drawing shits and values */
	ColorPicker( Color* m_Select, float* m_Hue, bool m_Alpha = false );

	void Draw( );
	void Update( );

	vec2_t m_Pos;
	vec2_t m_CursorPos;

private:
	/* values */
	bool m_Focused;
	bool m_AlphaBar;

	float* m_Hue;
	float m_Alpha;

	Color* m_Select;
};