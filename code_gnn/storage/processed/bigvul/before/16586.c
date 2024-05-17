int FileTransfer::InitializePlugins(CondorError &e) {

	if (!param_boolean("ENABLE_URL_TRANSFERS", true)) {
		I_support_filetransfer_plugins = false;
		return 0;
	}

	char* plugin_list_string = param("FILETRANSFER_PLUGINS");
	if (!plugin_list_string) {
		I_support_filetransfer_plugins = false;
		return 0;
	}

	plugin_table = new PluginHashTable(7, compute_filename_hash);

	StringList plugin_list (plugin_list_string);
	plugin_list.rewind();

	char *p;
	while ((p = plugin_list.next())) {

		MyString methods = DeterminePluginMethods(e, p);
		if (!methods.IsEmpty()) {
			I_support_filetransfer_plugins = true;
			InsertPluginMappings(methods, p);
		} else {
			dprintf(D_ALWAYS, "FILETRANSFER: failed to add plugin \"%s\" because: %s\n", p, e.getFullText());
		}
	}

	free(plugin_list_string);
	return 0;
}
