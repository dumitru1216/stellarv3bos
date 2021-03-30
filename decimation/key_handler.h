#pragma once

class c_keyhandler : public singleton<c_keyhandler> {
public:
	bool is_key_down(int key);
	bool is_key_up(int key);
	bool auto_check(int key, int style);
};
