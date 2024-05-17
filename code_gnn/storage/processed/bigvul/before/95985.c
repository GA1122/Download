void CL_Sayto_f( void ) {
	char		*rawname;
	char		name[MAX_NAME_LENGTH];
	char		cleanName[MAX_NAME_LENGTH];
	const char	*info;
	int			count;
	int			i;
	int			clientNum;
	char		*p;

	if ( Cmd_Argc() < 3 ) {
		Com_Printf ("sayto <player name> <text>\n");
		return;
	}

	rawname = Cmd_Argv(1);

	Com_FieldStringToPlayerName( name, MAX_NAME_LENGTH, rawname );

	info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO];
	count = atoi( Info_ValueForKey( info, "sv_maxclients" ) );

	clientNum = -1;
	for( i = 0; i < count; i++ ) {

		info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_PLAYERS+i];
		Q_strncpyz( cleanName, Info_ValueForKey( info, "n" ), sizeof(cleanName) );
		Q_CleanStr( cleanName );

		if ( !Q_stricmp( cleanName, name ) ) {
			clientNum = i;
			break;
		}
	}
	if( clientNum <= -1 )
	{
		Com_Printf ("No such player name: %s.\n", name);
		return;
	}

	p = Cmd_ArgsFrom(2);

	if ( *p == '"' ) {
		p++;
		p[strlen(p)-1] = 0;
	}

	CL_AddReliableCommand(va("tell %i \"%s\"", clientNum, p ), qfalse);
}
