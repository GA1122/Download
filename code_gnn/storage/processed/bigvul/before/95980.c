void CL_Record_f( void ) {
	char		name[MAX_OSPATH];
	byte		bufData[MAX_MSGLEN];
	msg_t	buf;
	int			i;
	int			len;
	entityState_t	*ent;
	entityState_t	nullstate;
	char		*s;

	if ( Cmd_Argc() > 2 ) {
		Com_Printf ("record <demoname>\n");
		return;
	}

	if ( clc.demorecording ) {
		if (!clc.spDemoRecording) {
			Com_Printf ("Already recording.\n");
		}
		return;
	}

	if ( clc.state != CA_ACTIVE ) {
		Com_Printf ("You must be in a level to record.\n");
		return;
	}

	if ( NET_IsLocalAddress( clc.serverAddress ) && !Cvar_VariableValue( "g_synchronousClients" ) ) {
		Com_Printf (S_COLOR_YELLOW "WARNING: You should set 'g_synchronousClients 1' for smoother demo recording\n");
	}

	if ( Cmd_Argc() == 2 ) {
		s = Cmd_Argv(1);
		Q_strncpyz( demoName, s, sizeof( demoName ) );
#ifdef LEGACY_PROTOCOL
		if(clc.compat)
			Com_sprintf(name, sizeof(name), "demos/%s.%s%d", demoName, DEMOEXT, com_legacyprotocol->integer);
		else
#endif
			Com_sprintf(name, sizeof(name), "demos/%s.%s%d", demoName, DEMOEXT, com_protocol->integer);
	} else {
		int		number;

		for ( number = 0 ; number <= 9999 ; number++ ) {
			CL_DemoFilename( number, demoName, sizeof( demoName ) );
#ifdef LEGACY_PROTOCOL
			if(clc.compat)
				Com_sprintf(name, sizeof(name), "demos/%s.%s%d", demoName, DEMOEXT, com_legacyprotocol->integer);
			else
#endif
				Com_sprintf(name, sizeof(name), "demos/%s.%s%d", demoName, DEMOEXT, com_protocol->integer);

			if (!FS_FileExists(name))
				break;	 
		}
	}


	Com_Printf ("recording to %s.\n", name);
	clc.demofile = FS_FOpenFileWrite( name );
	if ( !clc.demofile ) {
		Com_Printf ("ERROR: couldn't open.\n");
		return;
	}
	clc.demorecording = qtrue;
	if (Cvar_VariableValue("ui_recordSPDemo")) {
	  clc.spDemoRecording = qtrue;
	} else {
	  clc.spDemoRecording = qfalse;
	}

	Q_strncpyz( clc.demoName, demoName, sizeof( clc.demoName ) );

	clc.demowaiting = qtrue;

	MSG_Init (&buf, bufData, sizeof(bufData));
	MSG_Bitstream(&buf);

	MSG_WriteLong( &buf, clc.reliableSequence );

	MSG_WriteByte (&buf, svc_gamestate);
	MSG_WriteLong (&buf, clc.serverCommandSequence );

	for ( i = 0 ; i < MAX_CONFIGSTRINGS ; i++ ) {
		if ( !cl.gameState.stringOffsets[i] ) {
			continue;
		}
		s = cl.gameState.stringData + cl.gameState.stringOffsets[i];
		MSG_WriteByte (&buf, svc_configstring);
		MSG_WriteShort (&buf, i);
		MSG_WriteBigString (&buf, s);
	}

	Com_Memset (&nullstate, 0, sizeof(nullstate));
	for ( i = 0; i < MAX_GENTITIES ; i++ ) {
		ent = &cl.entityBaselines[i];
		if ( !ent->number ) {
			continue;
		}
		MSG_WriteByte (&buf, svc_baseline);		
		MSG_WriteDeltaEntity (&buf, &nullstate, ent, qtrue );
	}

	MSG_WriteByte( &buf, svc_EOF );
	

	MSG_WriteLong(&buf, clc.clientNum);
	MSG_WriteLong(&buf, clc.checksumFeed);

	MSG_WriteByte( &buf, svc_EOF );

	len = LittleLong( clc.serverMessageSequence - 1 );
	FS_Write (&len, 4, clc.demofile);

	len = LittleLong (buf.cursize);
	FS_Write (&len, 4, clc.demofile);
	FS_Write (buf.data, buf.cursize, clc.demofile);

}
