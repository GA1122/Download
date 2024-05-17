sysapi_opsys(void)
{
	if( ! arch_inited ) {
		init_arch();
	}
	return opsys;
}
