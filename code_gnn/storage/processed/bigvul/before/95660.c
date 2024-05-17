static void CL_CompleteRcon( char *args, int argNum )
{
	if( argNum == 2 )
	{
		char *p = Com_SkipTokens( args, 1, " " );

		if( p > args )
			Field_CompleteCommand( p, qtrue, qtrue );
	}
}
