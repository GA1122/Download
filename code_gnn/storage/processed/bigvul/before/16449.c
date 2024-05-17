int sysapi_opsys_version()
{
	if( ! arch_inited ) {
		init_arch();
	}
    return opsys_version;
}
