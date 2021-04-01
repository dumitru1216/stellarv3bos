#pragma once
#include "../m_groupbox/m_groupbox.h"

class Bind : public Base {
public:

	Bind( int* m_Value, int* m_BindType );

	void Update( );
	void Draw( );

	void UpdateList( );
	void InitList( );

	vec2_t m_Pos;

private:

	int* m_Value;
	int* m_BindType;

	bool m_Focused;
	bool m_FocusedList;
};