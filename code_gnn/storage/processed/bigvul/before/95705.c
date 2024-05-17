static __attribute__ ((format (printf, 2, 3))) void QDECL CL_RefPrintf( int print_level, const char *fmt, ...) {
	va_list	argptr;
	char	msg[MAXPRINTMSG];

	va_start( argptr,fmt );
	Q_vsnprintf( msg, sizeof ( msg ), fmt, argptr );
	va_end( argptr );

	if ( print_level == PRINT_ALL ) {
		Com_Printf( "%s", msg );
	} else if ( print_level == PRINT_WARNING ) {
		Com_Printf( S_COLOR_YELLOW "%s", msg );		 
	} else if ( print_level == PRINT_DEVELOPER ) {
		Com_DPrintf( S_COLOR_RED "%s", msg );		 
	}
}