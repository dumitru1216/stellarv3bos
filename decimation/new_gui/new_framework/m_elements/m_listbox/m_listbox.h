#pragma once
#include "../m_groupbox/m_groupbox.h"

class ListBox : public Base {
public:

	// Constructors.
	ListBox( std::string m_Name, int* m_Select, std::vector<std::string>& m_Items );
	ListBox( std::string m_Name, int* m_Select, std::vector<std::string>& m_Items, std::string& m_Filter );

	void Draw( );
	void Update( );

	vec2_t m_Pos;

private:

	void GetItems( );
	void Scroll( int m_Limit );

	std::string ToLower( std::string String );
	int* m_Select;

	std::vector<std::string> m_LowerItems;
	std::vector<std::string>* m_Items;

	std::string* m_Filter;
	std::string m_PreviousFilter = "123";

	std::string m_Name;
	std::vector< std::pair<std::string, int >> m_Temp;
};