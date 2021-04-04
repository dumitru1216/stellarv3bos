#ifndef hwid_list
#define hwid_list

#include <vector>
#include <utility>
#include <string>

enum user_types
{
	USER_TYPE_ROOT,	  //project owner
	USER_TYPE_DEV,	  //developer
	USER_TYPE_MOD,	  //moderator
	USER_TYPE_TRUSTED //trusted
};

static std::vector<std::pair<std::string, std::pair<std::string, int>>> hwids =
{ 
	{ "e27de7f38782f24e83de40b43ab70c2bde07eedb81307d1cdd921af0cf8c38e0", {"Dutu1337", USER_TYPE_DEV} },
	{ "e6f74fbc62ddf10287435b5785fa95befb8fc26e2bd52a53c6e4065bd6fd1a11", {"Hristo200", USER_TYPE_TRUSTED} },
	{ "48d799a6e716227bd53ecb40a792f59d8d364a5660b41d200f9187d807c309c2", {"cryzux", USER_TYPE_TRUSTED} },
	{ "2be4691bcd09e345f4ea42fa90876a0df5ded8330ab81dc5b84b738407bd0a4d", {"winter", USER_TYPE_TRUSTED} },
	{ "0b0ddb3fdbe5d2f1ae23ca00dbf035be6e54013825ef759e81a973e98a147a3f", {"Boryfff", USER_TYPE_TRUSTED} },
	{ "2877e7cbec33705d180730d2763cf448a54e2d12bafd19398281a01e08b92988", {"xRiro8", USER_TYPE_TRUSTED} },
	{ "df37cffa1862baa374ff8a1ecb77dd446c308e4299d25dec7809edced04143df", {"whynot.exe", USER_TYPE_TRUSTED} },
	{ "7bfc80ebec1400a318da0c71dfdea77c4fdb848fee09d6714f0850114704690d", {"Ascented", USER_TYPE_TRUSTED} },
	{ "59d0fa83f1ba7ebab99de1a6cf8221224bd5dea7a21e8a563f0175a69bfc9396", {"BoNrda", USER_TYPE_TRUSTED} },
	{ "15c90dfd8d1bd7fbd893fce5084f4f5e4e47ef6756d4198ce4e4185b841f9fbb", {"Lucretia4k", USER_TYPE_TRUSTED} },
	{ "1ba4106effd5c1d7999d790ecfa44485b6f76f91d5ed2fbe26e1f642f38cbf4d", {"Bppee", USER_TYPE_TRUSTED} },
	{ "369cd8ce2acae7a0e1964d564b38688e62101c2fa3aeb14b98ad48ce32d960ac", {"falsee", USER_TYPE_TRUSTED} },
	{ "d7cdbeb198e6e11c6443c78ef8871e50e505881f7354fd542d383d519c566102", {"xyren", USER_TYPE_TRUSTED} },
	{ "b289888935fb7d271b2be5a7ba4af852d09511d5312970999d39ef5c88d7ecb9", {"zodis", USER_TYPE_TRUSTED} },
	{ "31ef7054d3bf9773b490cbdf3c6bb73fbe4aa097a4a4fe034fb3066004c5f12c", {"kizum", USER_TYPE_TRUSTED} },
	{ "9883832fcd891dee53f6755718d359836b9677ba6242dd6a4cb64eb40018274b", {"eta", USER_TYPE_TRUSTED} },
	{ "bd6e05d4a5073ff3da68cd9a420b4ef3118b4efc06ce93ea4a6c1dc2e88fe343", {"barbarion", USER_TYPE_MOD} },
	{ "3b5d919365ea84799cb326fe7a01c1bcc6581b69f488d93d0c5f1bf2c1f804e7", {"saito", USER_TYPE_TRUSTED} },
	{ "0054e0c577ff5ad681598e3a137f849a548864d9cd227f1515668261d0196eec", {"soviet", USER_TYPE_TRUSTED} },
	{ "0a44edfb13599d4b907872baa4ad5c884a3ede4f06586d80b489e1afae56a9b1", {"shiny", USER_TYPE_TRUSTED} },
	{ "2768c11739869b6c0e85093e3c4dc1979c68998e8f3c0ee0363e342243ed7e33", {"doge", USER_TYPE_TRUSTED} },
	{ "75a3ba3d3ef50cb3640584f6baa3656ec6727a87f4b4b640b0a9b48b59eeef1b", {"cadua", USER_TYPE_TRUSTED} },

};

inline std::string user_type_to_string( int type )
{
	switch ( type )
	{
	case USER_TYPE_ROOT:
		return "admin";
	case USER_TYPE_DEV:
		return "dev";
	case USER_TYPE_MOD:
		return "alpha";
	case USER_TYPE_TRUSTED:
		return "beta";
	default:
		return "GTFO Cracker Faggot";
	}
}

#endif