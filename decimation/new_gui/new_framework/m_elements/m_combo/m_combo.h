#pragma once
#include "../m_groupbox/m_groupbox.h"

/* setup class for combo */
class Combo : public Base {
public:
	/* setup combo draw it and values */
	Combo( std::string m_Name, int* m_Value, std::vector<std::string> m_Items );

	void Update( );
	void Draw( );
	void DrawBox( );

	vec2_t m_Pos;
	bool m_Focused;

private:
	/* values */
	std::string m_Name;
	int* m_Value;
	
	/* items */
	std::vector<std::string> m_Items;
};