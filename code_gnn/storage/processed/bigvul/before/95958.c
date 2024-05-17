void CL_DownloadsComplete( void ) {

#ifdef USE_CURL
	if(clc.cURLUsed) { 
		clc.cURLUsed = qfalse;
		CL_cURL_Shutdown();
		if( clc.cURLDisconnected ) {
			if(clc.downloadRestart) {
				FS_Restart(clc.checksumFeed);
				clc.downloadRestart = qfalse;
			}
			clc.cURLDisconnected = qfalse;
			CL_Reconnect_f();
			return;
		}
	}
#endif

	if (clc.downloadRestart) {
		clc.downloadRestart = qfalse;

		FS_Restart(clc.checksumFeed);  

		CL_AddReliableCommand("donedl", qfalse);

		return;
	}

	clc.state = CA_LOADING;

	Com_EventLoop();

	if ( clc.state != CA_LOADING ) {
		return;
	}

	Cvar_Set("r_uiFullScreen", "0");

	CL_FlushMemory();

	cls.cgameStarted = qtrue;
	CL_InitCGame();

	CL_SendPureChecksums();

	CL_WritePacket();
	CL_WritePacket();
	CL_WritePacket();
}
