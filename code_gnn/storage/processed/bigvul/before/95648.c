void CL_CheckAutoUpdate( void ) {
	int validServerNum = 0;
	int i = 0, rnd = 0;
	netadr_t temp;
	char        *servername;

	if ( !cl_autoupdate->integer ) {
		return;
	}

	if ( autoupdateChecked ) {
		return;
	}

	srand( Com_Milliseconds() );

	for ( i = 0; i < MAX_AUTOUPDATE_SERVERS; i++ ) {
		if ( NET_StringToAdr( cls.autoupdateServerNames[i], &temp, NA_UNSPEC ) ) {
			validServerNum++;
		}
	}

	if ( validServerNum > 1 ) {
		rnd = rand() % validServerNum;
	} else {
		rnd = 0;
	}

	servername = cls.autoupdateServerNames[rnd];

	Com_DPrintf( "Resolving AutoUpdate Server... " );
	if ( !NET_StringToAdr( servername, &cls.autoupdateServer, NA_UNSPEC  ) ) {
		Com_DPrintf( "Couldn't resolve first address, trying default..." );

		if ( !NET_StringToAdr( cls.autoupdateServerNames[0], &cls.autoupdateServer, NA_UNSPEC  ) ) {
			Com_DPrintf( "Failed to resolve any Auto-update servers.\n" );
			autoupdateChecked = qtrue;
			return;
		}
	}
	cls.autoupdateServer.port = BigShort( PORT_SERVER );
	Com_DPrintf( "%i.%i.%i.%i:%i\n", cls.autoupdateServer.ip[0], cls.autoupdateServer.ip[1],
				 cls.autoupdateServer.ip[2], cls.autoupdateServer.ip[3],
				 BigShort( cls.autoupdateServer.port ) );

	NET_OutOfBandPrint( NS_CLIENT, cls.autoupdateServer, "getUpdateInfo \"%s\" \"%s\"-\"%s\"\n", Q3_VERSION, OS_STRING, ARCH_STRING );

	CL_RequestMotd();

	autoupdateChecked = qtrue;
}
