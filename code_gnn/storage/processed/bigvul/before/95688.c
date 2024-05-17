void CL_LocalServers_f( void ) {
	char	*message;
	int	i, j;
	netadr_t	to;

	Com_Printf( "Scanning for servers on the local network...\n" );

	cls.numlocalservers = 0;
	cls.pingUpdateSource = AS_LOCAL;

	for ( i = 0; i < MAX_OTHER_SERVERS; i++ ) {
		qboolean b = cls.localServers[i].visible;
		Com_Memset( &cls.localServers[i], 0, sizeof( cls.localServers[i] ) );
		cls.localServers[i].visible = b;
	}
	Com_Memset( &to, 0, sizeof( to ) );

	message = "\377\377\377\377getinfo xxx";

	for ( i = 0 ; i < 2 ; i++ ) {
		for ( j = 0 ; j < NUM_SERVER_PORTS ; j++ ) {
			to.port = BigShort( (short)( PORT_SERVER + j ) );

			to.type = NA_BROADCAST;
			NET_SendPacket( NS_CLIENT, strlen( message ), message, to );
			to.type = NA_MULTICAST6;
			NET_SendPacket( NS_CLIENT, strlen( message ), message, to );
		}
	}
}
