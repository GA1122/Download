int web_server_set_alias(const char *alias_name, 
	const char *alias_content, size_t alias_content_length,
	time_t last_modified)
{
	int ret_code;
	struct xml_alias_t alias;

	alias_release(&gAliasDoc);
	if (alias_name == NULL) {
		 
		return 0;
	}
	assert(alias_content != NULL);
	membuffer_init(&alias.doc);
	membuffer_init(&alias.name);
	alias.ct = NULL;
	do {
		 
		if (*alias_name != '/')
			if (membuffer_assign_str(&alias.name, "/") != 0)
				break;	 
		ret_code = membuffer_append_str(&alias.name, alias_name);
		if (ret_code != 0)
			break;	 
		if ((alias.ct = (int *)malloc(sizeof(int))) == NULL)
			break;	 
		*alias.ct = 1;
		membuffer_attach(&alias.doc, (char *)alias_content,
				 alias_content_length);
		alias.last_modified = last_modified;
		 
		ithread_mutex_lock(&gWebMutex);
		gAliasDoc = alias;
		ithread_mutex_unlock(&gWebMutex);

		return 0;
	} while (FALSE);
	 
	 
	membuffer_destroy(&alias.name);
	membuffer_destroy(&alias.doc);
	free(alias.ct);

	return UPNP_E_OUTOF_MEMORY;
}
