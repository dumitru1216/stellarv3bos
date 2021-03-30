#pragma once
#include "../m_groupbox/m_groupbox.h"

/* multibox constructor */
struct MultiConstructor {
	MultiConstructor( std::string m_Name, bool* m_Value ) {
		this->m_Name = m_Name;
		this->m_Value = m_Value;
	}

	/* values */
	std::string m_Name;
	bool* m_Value;
};

/* setup class of multibox */
class Multi : public Base {
public:
	/* multibox */
	Multi( );
	Multi( std::string m_Name );

	/* draw this */
	void Update( );
	void Draw( );
	void DrawBox( );

	void AddItem( std::string m_Name, bool* m_Value );

	vec2_t m_Pos;

private:
	/* values */
	std::string m_Name;
	std::vector<MultiConstructor> m_Items;

	bool m_Focused;
};