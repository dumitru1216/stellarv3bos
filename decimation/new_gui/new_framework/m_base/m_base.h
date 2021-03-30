#pragma once
#include "../../m_elements.h"

/* base class */
class Base {
	virtual void Destroy( Base* m_Element );
};
extern Base m_Base;