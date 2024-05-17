static void CL_SetServerInfo( serverInfo_t *server, const char *info, int ping ) {
	if ( server ) {
		if ( info ) {
			server->clients = atoi( Info_ValueForKey( info, "clients" ) );
			Q_strncpyz( server->hostName,Info_ValueForKey( info, "hostname" ), MAX_NAME_LENGTH );
			Q_strncpyz( server->mapName, Info_ValueForKey( info, "mapname" ), MAX_NAME_LENGTH );
			server->maxClients = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
			Q_strncpyz( server->game,Info_ValueForKey( info, "game" ), MAX_NAME_LENGTH );
			server->gameType = atoi( Info_ValueForKey( info, "gametype" ) );
			server->netType = atoi( Info_ValueForKey( info, "nettype" ) );
			server->minPing = atoi( Info_ValueForKey( info, "minping" ) );
			server->maxPing = atoi( Info_ValueForKey( info, "maxping" ) );
			server->allowAnonymous = atoi( Info_ValueForKey( info, "sv_allowAnonymous" ) );
			server->friendlyFire = atoi( Info_ValueForKey( info, "friendlyFire" ) );          
			server->maxlives = atoi( Info_ValueForKey( info, "maxlives" ) );                  
			server->tourney = atoi( Info_ValueForKey( info, "tourney" ) );                        
			server->punkbuster = atoi( Info_ValueForKey( info, "punkbuster" ) );              
			Q_strncpyz( server->gameName, Info_ValueForKey( info, "gamename" ), MAX_NAME_LENGTH );    
			server->antilag = atoi( Info_ValueForKey( info, "g_antilag" ) );
			server->g_humanplayers = atoi( Info_ValueForKey( info, "g_humanplayers" ) );
			server->g_needpass = atoi( Info_ValueForKey( info, "g_needpass" ) );
		}
		server->ping = ping;
	}
}
