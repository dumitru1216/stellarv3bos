#pragma once
#include "../m_groupbox/m_groupbox.h"

class Button : public Base {
public:

	Button( std::string m_Name, std::function< void( ) > Function );

	void Draw( );
	void Update( );

	vec2_t m_Pos;

private:

	std::string m_Name;
	std::function< void( ) > Function = { };

	bool m_Focused;
};