static void CL_CompleteDemoName( char *args, int argNum )
{
	if( argNum == 2 )
	{
		char demoExt[ 16 ];

		Com_sprintf(demoExt, sizeof(demoExt), ".%s%d", DEMOEXT, com_protocol->integer);
		Field_CompleteFilename( "demos", demoExt, qtrue, qtrue );
	}
}
