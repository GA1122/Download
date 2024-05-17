int FileTransfer::InvokeFileTransferPlugin(CondorError &e, const char* source, const char* dest, const char* proxy_filename) {

	if (plugin_table == NULL) {
		dprintf(D_FULLDEBUG, "FILETRANSFER: No plugin table defined! (request was %s)\n", source);
		e.pushf("FILETRANSFER", 1, "No plugin table defined (request was %s)", source);
		return GET_FILE_PLUGIN_FAILED;
	}


	char *URL = NULL;

	if(IsUrl(dest)) {
		URL = const_cast<char*>(dest);
		dprintf(D_FULLDEBUG, "FILETRANSFER: using destination to determine plugin type: %s\n", dest);
	} else {
		URL = const_cast<char*>(source);
		dprintf(D_FULLDEBUG, "FILETRANSFER: using source to determine plugin type: %s\n", source);
	}

	const char* colon = strchr(URL, ':');

	if (!colon) {
		e.pushf("FILETRANSFER", 1, "Specified URL does not contain a ':' (%s)", URL);
		return GET_FILE_PLUGIN_FAILED;
	}

	char* method = (char*) malloc(1 + (colon-URL));
	strncpy(method, URL, (colon-URL));
	method[(colon-URL)] = '\0';


	MyString plugin;

	if (plugin_table->lookup((MyString)method, plugin)) {
		e.pushf("FILETRANSFER", 1, "FILETRANSFER: plugin for type %s not found!", method);
		dprintf (D_FULLDEBUG, "FILETRANSFER: plugin for type %s not found!\n", method);
		free(method);
		return GET_FILE_PLUGIN_FAILED;
	}

	
 

	Env plugin_env;

	plugin_env.Import();

	if (proxy_filename && *proxy_filename) {
		plugin_env.SetEnv("X509_USER_PROXY",proxy_filename);
		dprintf(D_FULLDEBUG, "FILETRANSFER: setting X509_USER_PROXY env to %s\n", proxy_filename);
	}

	ArgList plugin_args;
	plugin_args.AppendArg(plugin.Value());
	plugin_args.AppendArg(source);
	plugin_args.AppendArg(dest);
	dprintf(D_FULLDEBUG, "FILETRANSFER: invoking: %s %s %s\n", plugin.Value(), source, dest);

	FILE* plugin_pipe = my_popen(plugin_args, "r", FALSE, &plugin_env);
	int plugin_status = my_pclose(plugin_pipe);

	dprintf (D_ALWAYS, "FILETRANSFER: plugin returned %i\n", plugin_status);

	free(method);

	if (plugin_status != 0) {
		e.pushf("FILETRANSFER", 1, "non-zero exit(%i) from %s\n", plugin_status, plugin.Value());
		return GET_FILE_PLUGIN_FAILED;
	}

	return 0;
}
