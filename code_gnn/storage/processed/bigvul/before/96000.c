void CL_StopRecord_f( void ) {
	int		len;

	if ( !clc.demorecording ) {
		Com_Printf ("Not recording a demo.\n");
		return;
	}

	len = -1;
	FS_Write (&len, 4, clc.demofile);
	FS_Write (&len, 4, clc.demofile);
	FS_FCloseFile (clc.demofile);
	clc.demofile = 0;
	clc.demorecording = qfalse;
	clc.spDemoRecording = qfalse;
	Com_Printf ("Stopped demo.\n");
}