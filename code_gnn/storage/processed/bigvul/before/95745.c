void CL_Video_f( void )
{
	char  filename[ MAX_OSPATH ];
	int   i, last;

	if( !clc.demoplaying )
	{
		Com_Printf( "The video command can only be used when playing back demos\n" );
		return;
	}

	if( Cmd_Argc( ) == 2 )
	{
		Com_sprintf( filename, MAX_OSPATH, "videos/%s.avi", Cmd_Argv( 1 ) );
	}
	else
	{
		for( i = 0; i <= 9999; i++ )
		{
			int a, b, c, d;

			last = i;

			a = last / 1000;
			last -= a * 1000;
			b = last / 100;
			last -= b * 100;
			c = last / 10;
			last -= c * 10;
			d = last;

			Com_sprintf( filename, MAX_OSPATH, "videos/video%d%d%d%d.avi", a, b, c, d );

			if( !FS_FileExists( filename ) )
			break;  
		}

		if( i > 9999 )
		{
			Com_Printf( S_COLOR_RED "ERROR: no free file names to create video\n" );
			return;
		}
	}

	CL_OpenAVIForWriting( filename );
}
