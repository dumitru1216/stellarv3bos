#pragma once
#include "../m_groupbox/m_groupbox.h"

class Checkbox : public Base {
public:

	// Constructor.
	Checkbox( std::string m_Name, bool* m_Value );

	void Draw( );
	void Update( );

	vec2_t m_Pos;

private:

	std::string m_Name;
	bool* m_Value;
};