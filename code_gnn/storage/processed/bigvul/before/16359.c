static int access_via_nfs( const char *   )
{
	char *my_uid_domain=0;
	char *my_fs_domain=0;
	char *remote_uid_domain=0;
	char *remote_fs_domain=0;
	int result = 0;

	dprintf( D_SYSCALLS, "\tentering access_via_nfs()\n" );

	my_uid_domain = param("UID_DOMAIN");
	my_fs_domain = param("FILESYSTEM_DOMAIN");

	thisRemoteResource->getUidDomain(remote_uid_domain);
	thisRemoteResource->getFilesystemDomain(remote_fs_domain);

	if( !param_boolean_crufty("USE_NFS", false) ) {
		dprintf( D_SYSCALLS, "\tnot configured to use NFS for file access\n" );
		goto done;
	}

	if( !my_uid_domain ) {
		dprintf( D_SYSCALLS, "\tdon't know my UID domain\n" );
		goto done;
	}

	if( !my_fs_domain ) {
		dprintf( D_SYSCALLS, "\tdon't know my FS domain\n" );
		goto done;
	}

	if( !remote_uid_domain ) {
		dprintf( D_SYSCALLS, "\tdon't know UID domain of executing machine\n" );
		goto done;
	}

	if( !remote_fs_domain ) {
		dprintf( D_SYSCALLS, "\tdon't know FS domain of executing machine\n" );
		goto done;
	}

	dprintf( D_SYSCALLS,
		"\tMy_FS_Domain = \"%s\", Executing_FS_Domain = \"%s\"\n",
		my_fs_domain,
		remote_fs_domain
	);

	dprintf( D_SYSCALLS,
		"\tMy_UID_Domain = \"%s\", Executing_UID_Domain = \"%s\"\n",
		my_uid_domain,
		remote_uid_domain
	);

	if( strcmp(my_fs_domain,remote_fs_domain) != MATCH ) {
		dprintf( D_SYSCALLS, "\tFilesystem domains don't match\n" );
		goto done;
	}

	if( strcmp(my_uid_domain,remote_uid_domain) != MATCH ) {
		dprintf( D_SYSCALLS, "\tUID domains don't match\n" );
		goto done;
	}

	result = 1;

	done:
	dprintf( D_SYSCALLS, "\taccess_via_NFS() returning %s\n", result ? "TRUE" : "FALSE" );
	if (remote_fs_domain) free(remote_fs_domain);
	if (remote_uid_domain) free(remote_uid_domain);
	return result;
}
