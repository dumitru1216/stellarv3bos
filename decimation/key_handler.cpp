#include "includes.h"
#include "key_handler.h"

bool c_keyhandler::is_key_down(int key) {
	return HIWORD(GetKeyState(key));
}

bool c_keyhandler::is_key_up(int key) {
	return !HIWORD(GetKeyState(key));
}

bool c_keyhandler::auto_check(int key, int style) {
	switch (style) {
	case 0:
		return true;
	case 1:
		return is_key_down(key);
	case 2:
		return LOWORD(GetKeyState(key));
	case 3:
		return is_key_up(key);
	default:
		return true;
	}
}