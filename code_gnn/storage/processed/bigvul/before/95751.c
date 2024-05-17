void CL_startSingleplayer_f( void ) {
	char binName[MAX_OSPATH];

#if defined(_WIN64) || defined(__WIN64__)
	Com_sprintf(binName, sizeof(binName), "ioWolfSP." ARCH_STRING ".exe");
	Sys_StartProcess( binName, qtrue );
#elif defined(_WIN32) || defined(__WIN32__)
	Com_sprintf(binName, sizeof(binName), "ioWolfSP." BIN_STRING ".exe");
	Sys_StartProcess( binName, qtrue );
#elif defined(__i386__) && (!defined(_WIN32) || !defined(__WIN32__))
	Com_sprintf(binName, sizeof(binName), "./iowolfsp." BIN_STRING );
	Sys_StartProcess( binName, qtrue );
#else
	Com_sprintf(binName, sizeof(binName), "./iowolfsp." ARCH_STRING );
	Sys_StartProcess( binName, qtrue );
#endif
}
