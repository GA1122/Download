init_arch(void)
{
	struct utsname buf;

	if( uname(&buf) < 0 ) {
		return;
	}

	uname_arch = strdup( buf.machine );
	if( !uname_arch ) {
		EXCEPT( "Out of memory!" );
	}

	uname_opsys = strdup( buf.sysname );
	if( !uname_opsys ) {
		EXCEPT( "Out of memory!" );
	}

	arch = sysapi_translate_arch( buf.machine, buf.sysname );
	opsys = sysapi_translate_opsys( buf.sysname, buf.release, buf.version, _sysapi_opsys_is_versioned );
	opsys_versioned = sysapi_translate_opsys( buf.sysname, buf.release, buf.version, true );
    opsys_version = sysapi_translate_opsys_version( buf.sysname, buf.release, buf.version );

	if ( arch && opsys ) {
		arch_inited = TRUE;
	}
}
