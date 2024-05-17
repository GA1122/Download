sysapi_uname_arch(void)
{
	if( ! arch_inited ) {
		init_arch();
	}
	return uname_arch;
}
