#pragma once

/* include type c++ */
#include <vector>
#include <string>

/* include type file */
#include "../../m_base/m_base.h"
#include "../m_groupbox/m_groupbox.h"

/* dinamic class */
class Groupbox;

/* window class */
class Window : public Base {
	/* public selection */
public:
	/* public forms which i can use in menu */
	Window( std::string m_Name, vec2_t* m_Pos, vec2_t m_Size, std::vector<std::string> m_WindowTabs = { }, int* m_Selected = 0 );
	void AddGroup( Groupbox* Groupbox );
	vec2_t* m_WindowPosition;

	/* tab thing */
	std::vector<std::string> m_WindowTabs;
	int* m_Selected;
	/* private selection */
private:
	/* menu form */
	void Draw( );
	void Run( );

	/* tab thing */
	void InitTabs( );
	void InitPosition( );
	bool m_SetSwitch;

	/* other form */
	std::string m_Name;
	vec2_t m_Size;
};