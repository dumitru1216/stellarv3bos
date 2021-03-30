#pragma once
#include "../m_groupbox/m_groupbox.h"

class TextInput : public Base {
public:

	TextInput( std::string* m_Label );

	void Update( );
	void Draw( );

	vec2_t m_Pos;

private:

	std::string* m_Label;
	bool m_Focused;

};