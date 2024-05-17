void CL_DownloadsComplete( void ) {

#ifndef _WIN32
	char    *fs_write_path;
#endif
	char    *fn;

	if ( autoupdateStarted ) {

		if ( strlen( autoupdateFilename ) > 4 )  {
#ifdef _WIN32
			fn = va( "%s/%s", FS_ShiftStr( AUTOUPDATE_DIR, AUTOUPDATE_DIR_SHIFT ), autoupdateFilename );
#else
			fs_write_path = Cvar_VariableString( "fs_homepath" );
			fn = FS_BuildOSPath( fs_write_path, FS_ShiftStr( AUTOUPDATE_DIR, AUTOUPDATE_DIR_SHIFT ), autoupdateFilename );
#ifdef __linux__
			Sys_Chmod( fn, S_IXUSR );
#endif
#endif
			Sys_StartProcess( fn, qtrue );
		}

		autoupdateStarted = qfalse;
		CL_Disconnect( qtrue );
		return;
	}

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

	if ( clc.downloadRestart ) {
		clc.downloadRestart = qfalse;

		FS_Restart( clc.checksumFeed );  

		CL_AddReliableCommand( "donedl", qfalse );

		return;
	}

	clc.state = CA_LOADING;

	Com_EventLoop();

	if ( clc.state != CA_LOADING ) {
		return;
	}

	Cvar_Set( "r_uiFullScreen", "0" );

	CL_FlushMemory();

	cls.cgameStarted = qtrue;
	CL_InitCGame();

	CL_SendPureChecksums();

	CL_WritePacket();
	CL_WritePacket();
	CL_WritePacket();
}
