sysapi_uname_opsys(void)
{
	if( ! arch_inited ) {
		init_arch();
	}
	return uname_opsys;
}
