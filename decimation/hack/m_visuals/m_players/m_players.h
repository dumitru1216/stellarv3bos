#pragma once

class m_visuals {
public:
	/* strings */
	std::array< bool, 64 > m_draw_player;
	std::array< float, 2048 > m__modern_opacities;

	/* drawing */
	void draw_enemy( Player* player );
	void draw_visuals( Entity* ent );
};
extern m_visuals visuals;