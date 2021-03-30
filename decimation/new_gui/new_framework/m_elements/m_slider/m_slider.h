#pragma once
#include "../m_groupbox/m_groupbox.h"

/* installing slider class */
class Slider : public Base {
public:
	Slider( std::string m_Name, float* m_Value, int m_Min = 0, int m_Max = 100, std::string m_Suffix = "" );

	/* drawing and positions */
	void Draw( );
	void Update( );

	vec2_t m_Pos;
	bool m_AllowDraw;

private:
	/* values */
	std::string m_Name;
	std::string m_Suffix;
	float* m_Value;
	int m_Min;
	int m_Max;
};