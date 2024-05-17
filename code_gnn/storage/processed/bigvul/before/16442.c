get_hpux_arch( void )
{
	 
#if defined(IS_IA64_HPUX)
	return strdup("IA64");
#else
	return strdup("HPPA2");
#endif

}
