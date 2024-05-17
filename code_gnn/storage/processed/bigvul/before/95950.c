static void CL_CompletePlayerName( char *args, int argNum )
{
	if( argNum == 2 )
	{
		char		names[MAX_CLIENTS][MAX_NAME_LENGTH];
		const char	*namesPtr[MAX_CLIENTS];
		int			i;
		int			clientCount;
		int			nameCount;
		const char *info;
		const char *name;

		info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO];
		clientCount = atoi( Info_ValueForKey( info, "sv_maxclients" ) );

		nameCount = 0;

		for( i = 0; i < clientCount; i++ ) {
			if( i == clc.clientNum )
				continue;

			info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_PLAYERS+i];

			name = Info_ValueForKey( info, "n" );
			if( name[0] == '\0' )
				continue;
			Q_strncpyz( names[nameCount], name, sizeof(names[nameCount]) );
			Q_CleanStr( names[nameCount] );

			namesPtr[nameCount] = names[nameCount];
			nameCount++;
		}
		qsort( (void*)namesPtr, nameCount, sizeof( namesPtr[0] ), Com_strCompare );

		Field_CompletePlayerName( namesPtr, nameCount );
	}
}
