make_syslog_ident(const char* name)
{
	size_t ident_len = strlen(name) + 1;
	char *ident;

#if HAVE_DECL_CLONE_NEWNET
	if (global_data->network_namespace)
		ident_len += strlen(global_data->network_namespace) + 1;
#endif
	if (global_data->instance_name)
		ident_len += strlen(global_data->instance_name) + 1;

	 
#ifndef _MEM_CHECK_LOG_
	ident = MALLOC(ident_len);
#else
	ident = malloc(ident_len);
#endif

	if (!ident)
		return NULL;

	strcpy(ident, name);
#if HAVE_DECL_CLONE_NEWNET
	if (global_data->network_namespace) {
		strcat(ident, "_");
		strcat(ident, global_data->network_namespace);
	}
#endif
	if (global_data->instance_name) {
		strcat(ident, "_");
		strcat(ident, global_data->instance_name);
	}

	return ident;
}