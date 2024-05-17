sysapi_condor_arch(void)
{
	if( ! arch_inited ) {
		init_arch();
	}
	return arch;
}
