void CL_Vid_Restart_f( void ) {

	Cvar_Set( "com_expectedhunkusage", "-1" );

	if( CL_VideoRecording( ) ) {
		CL_CloseAVI( );
	}

	if(clc.demorecording)
		CL_StopRecord_f();

	S_StopAllSounds();

	if(!FS_ConditionalRestart(clc.checksumFeed, qtrue))
	{
		if(com_sv_running->integer)
		{
			Hunk_ClearToMark();
		}
		else
		{
			Hunk_Clear();
		}

		CL_ShutdownUI();
		CL_ShutdownCGame();
		CL_ShutdownRef();
		CL_ResetPureClientAtServer();
		FS_ClearPakReferences( FS_UI_REF | FS_CGAME_REF );

		S_BeginRegistration();   

		cls.rendererStarted = qfalse;
		cls.uiStarted = qfalse;
		cls.cgameStarted = qfalse;
		cls.soundRegistered = qfalse;
		autoupdateChecked = qfalse;

		Cvar_Set( "cl_paused", "0" );

		CL_InitRef();

		CL_StartHunkUsers(qfalse);

		if(clc.state > CA_CONNECTED && clc.state != CA_CINEMATIC)
		{
			cls.cgameStarted = qtrue;
			CL_InitCGame();
			CL_SendPureChecksums();
		}

	}
}
