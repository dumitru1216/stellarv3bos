#include "../../../includes.h"

m_visuals visuals{ };;

void m_visuals::draw_visuals( Entity* ent ) {
	if ( ent->IsPlayer( ) ) {
		Player* player = ent->as< Player* >( );

		// dont draw dead players.
		if ( !player->alive( ) )
			return;

		if ( player->m_bIsLocalPlayer( ) )
			return;

		// draw player esp.
		draw_enemy( player );
	}
}

void m_visuals::draw_enemy( Player* player ) {

}