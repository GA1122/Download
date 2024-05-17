void CL_GetAutoUpdate( void ) {

	if ( !autoupdateChecked ) {
		return;
	}

	if ( strlen( cl_updatefiles->string ) < 5 ) {
		return;
	}

	Com_DPrintf( "Connecting to auto-update server...\n" );

	S_StopAllSounds();       

	Cvar_Set( "r_uiFullScreen", "0" );

	clc.serverMessage[0] = 0;

	if ( com_sv_running->integer ) {
		SV_Shutdown( "Server quit\n" );
	}

	Cvar_Set( "sv_killserver", "1" );
	SV_Frame( 0 );

	CL_Disconnect( qtrue );
	Con_Close();

	Q_strncpyz( clc.servername, "Auto-Updater", sizeof( clc.servername ) );

	if ( cls.autoupdateServer.type == NA_BAD ) {
		Com_Printf( "Bad server address\n" );
		clc.state = CA_DISCONNECTED;
		return;
	}

	memcpy( &clc.serverAddress, &cls.autoupdateServer, sizeof( netadr_t ) );

	Com_DPrintf( "%s resolved to %i.%i.%i.%i:%i\n", clc.servername,
				 clc.serverAddress.ip[0], clc.serverAddress.ip[1],
				 clc.serverAddress.ip[2], clc.serverAddress.ip[3],
				 BigShort( clc.serverAddress.port ) );

	clc.state = CA_CONNECTING;

	Key_SetCatcher( 0 );
	clc.connectTime = -99999;    
	clc.connectPacketCount = 0;

	Cvar_Set( "cl_currentServerAddress", "Auto-Updater" );
}
