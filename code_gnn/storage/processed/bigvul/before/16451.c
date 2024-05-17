sysapi_opsys_versioned(void)
{
	if( ! arch_inited ) {
		init_arch();
	}
	return opsys_versioned;
}
