void CL_startMultiplayer_f( void ) {
	char binName[MAX_OSPATH];

#if defined(_WIN64) || defined(__WIN64__)
	Com_sprintf(binName, sizeof(binName), "ioWolfMP." ARCH_STRING ".exe");
	Sys_StartProcess( binName, qtrue );
#elif defined(_WIN32) || defined(__WIN32__)
	Com_sprintf(binName, sizeof(binName), "ioWolfMP." ARCH_STRING ".exe");
	Sys_StartProcess( binName, qtrue );
#elif defined(__i386__) && (!defined(_WIN32) || !defined(__WIN32__))
	Com_sprintf(binName, sizeof(binName), "./iowolfmp." BIN_STRING );
	Sys_StartProcess( binName, qtrue );
#else
	Com_sprintf(binName, sizeof(binName), "./iowolfmp." ARCH_STRING );
	Sys_StartProcess( binName, qtrue );
#endif
}
