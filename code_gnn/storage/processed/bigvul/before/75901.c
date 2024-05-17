static bool reload_config(void)
{
	bool unsupported_change = false;

	log_message(LOG_INFO, "Reloading ...");

	 
	old_global_data = global_data;
	global_data = NULL;
	global_data = alloc_global_data();

	read_config_file();

	init_global_data(global_data, old_global_data);

#if HAVE_DECL_CLONE_NEWNET
	if (!!old_global_data->network_namespace != !!global_data->network_namespace ||
	    (global_data->network_namespace && strcmp(old_global_data->network_namespace, global_data->network_namespace))) {
		log_message(LOG_INFO, "Cannot change network namespace at a reload - please restart %s", PACKAGE);
		unsupported_change = true;
	}
	FREE_PTR(global_data->network_namespace);
	global_data->network_namespace = old_global_data->network_namespace;
	old_global_data->network_namespace = NULL;
#endif

	if (!!old_global_data->instance_name != !!global_data->instance_name ||
	    (global_data->instance_name && strcmp(old_global_data->instance_name, global_data->instance_name))) {
		log_message(LOG_INFO, "Cannot change instance name at a reload - please restart %s", PACKAGE);
		unsupported_change = true;
	}
	FREE_PTR(global_data->instance_name);
	global_data->instance_name = old_global_data->instance_name;
	old_global_data->instance_name = NULL;

	if (unsupported_change) {
		 
		free_global_data (global_data);
		global_data = old_global_data;
	}
	else
		free_global_data (old_global_data);

	return !unsupported_change;
}