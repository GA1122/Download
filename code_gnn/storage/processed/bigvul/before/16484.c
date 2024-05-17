check_domain_attributes()
{
		 

	char *uid_domain, *filesys_domain;

	filesys_domain = param("FILESYSTEM_DOMAIN");
	if( !filesys_domain ) {
		insert( "FILESYSTEM_DOMAIN", get_local_fqdn().Value(), 
				ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("FILESYSTEM_DOMAIN");
	} else {
		free( filesys_domain );
	}

	uid_domain = param("UID_DOMAIN");
	if( !uid_domain ) {
		insert( "UID_DOMAIN", get_local_fqdn().Value(), 
				ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("UID_DOMAIN");
	} else {
		free( uid_domain );
	}
}
