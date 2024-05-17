static void CL_SetServerInfoByAddress( netadr_t from, const char *info, int ping ) {
	int i;

	for ( i = 0; i < MAX_OTHER_SERVERS; i++ ) {
		if ( NET_CompareAdr( from, cls.localServers[i].adr ) ) {
			CL_SetServerInfo( &cls.localServers[i], info, ping );
		}
	}

	for ( i = 0; i < MAX_GLOBAL_SERVERS; i++ ) {
		if ( NET_CompareAdr( from, cls.globalServers[i].adr ) ) {
			CL_SetServerInfo( &cls.globalServers[i], info, ping );
		}
	}

	for ( i = 0; i < MAX_OTHER_SERVERS; i++ ) {
		if ( NET_CompareAdr( from, cls.favoriteServers[i].adr ) ) {
			CL_SetServerInfo( &cls.favoriteServers[i], info, ping );
		}
	}

}