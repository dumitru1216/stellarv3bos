#include "m_base.h"

/* m_base */
Base m_Base;
void Base::Destroy( Base* m_Element ) {
	delete m_Element;
}