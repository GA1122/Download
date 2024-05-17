void CL_PlayDemo_f( void ) {
	char		name[MAX_OSPATH];
	char		*arg, *ext_test;
	int			protocol, i;
	char		retry[MAX_OSPATH];

	if (Cmd_Argc() != 2) {
		Com_Printf ("demo <demoname>\n");
		return;
	}

	Cvar_Set( "sv_killserver", "2" );

	arg = Cmd_Argv(1);
	
	CL_Disconnect( qtrue );

	ext_test = strrchr(arg, '.');
	
	if(ext_test && !Q_stricmpn(ext_test + 1, DEMOEXT, ARRAY_LEN(DEMOEXT) - 1))
	{
		protocol = atoi(ext_test + ARRAY_LEN(DEMOEXT));

		for(i = 0; demo_protocols[i]; i++)
		{
			if(demo_protocols[i] == protocol)
				break;
		}

		if(demo_protocols[i] || protocol == com_protocol->integer
#ifdef LEGACY_PROTOCOL
		   || protocol == com_legacyprotocol->integer
#endif
		  )
		{
			Com_sprintf(name, sizeof(name), "demos/%s", arg);
			FS_FOpenFileRead(name, &clc.demofile, qtrue);
		}
		else
		{
			int len;

			Com_Printf("Protocol %d not supported for demos\n", protocol);
			len = ext_test - arg;

			if(len >= ARRAY_LEN(retry))
				len = ARRAY_LEN(retry) - 1;

			Q_strncpyz(retry, arg, len + 1);
			retry[len] = '\0';
			protocol = CL_WalkDemoExt(retry, name, &clc.demofile);
		}
	}
	else
		protocol = CL_WalkDemoExt(arg, name, &clc.demofile);
	
	if (!clc.demofile) {
		Com_Error( ERR_DROP, "couldn't open %s", name);
		return;
	}
	Q_strncpyz( clc.demoName, arg, sizeof( clc.demoName ) );

	Con_Close();

	clc.state = CA_CONNECTED;
	clc.demoplaying = qtrue;
	Q_strncpyz( clc.servername, arg, sizeof( clc.servername ) );

#ifdef LEGACY_PROTOCOL
	if(protocol <= com_legacyprotocol->integer)
		clc.compat = qtrue;
	else
		clc.compat = qfalse;
#endif

	while ( clc.state >= CA_CONNECTED && clc.state < CA_PRIMED ) {
		CL_ReadDemoMessage();
	}
	clc.firstDemoFrameSkipped = qfalse;
}
