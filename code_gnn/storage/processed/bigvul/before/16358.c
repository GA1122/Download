static int access_via_afs( const char *   )
{
	char *my_fs_domain=0;
	char *remote_fs_domain=0;
	int result=0;

	dprintf( D_SYSCALLS, "\tentering access_via_afs()\n" );

	my_fs_domain = param("FILESYSTEM_DOMAIN");
	thisRemoteResource->getFilesystemDomain(remote_fs_domain);

	if(!param_boolean_crufty("USE_AFS", false)) {
		dprintf( D_SYSCALLS, "\tnot configured to use AFS for file access\n" );
		goto done;
	}

	if(!my_fs_domain) {
		dprintf( D_SYSCALLS, "\tmy FILESYSTEM_DOMAIN is not defined\n" );
		goto done;
	}

	if(!remote_fs_domain) {
		dprintf( D_SYSCALLS, "\tdon't know FILESYSTEM_DOMAIN of executing machine\n" );
		goto done;
	}

	dprintf( D_SYSCALLS,
		"\tMy_FS_Domain = \"%s\", Executing_FS_Domain = \"%s\"\n",
		my_fs_domain,
		remote_fs_domain
	);

	if( strcmp(my_fs_domain,remote_fs_domain) != MATCH ) {
		dprintf( D_SYSCALLS, "\tFilesystem domains don't match\n" );
		goto done;
	}

	result = 1;

	done:
	dprintf(D_SYSCALLS,"\taccess_via_afs() returning %s\n", result ? "TRUE" : "FALSE" );
	if(my_fs_domain) free(my_fs_domain);
	if(remote_fs_domain) free(remote_fs_domain);
	return result;
}
