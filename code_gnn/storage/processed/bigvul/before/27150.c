int main(int argc, char **argv)
{
  const char *plugin_path = NULL;
  const char *connection_path = NULL;

  enum {
	CMD_RUN,
	CMD_TEST,
	CMD_INFO,
	CMD_HELP
  };
  int cmd = CMD_RUN;

  for (int i = 0; i < argc; i++) {
	const char *arg = argv[i];
	if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
	  argv[i] = NULL;
	  cmd = CMD_HELP;
	}
	else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--test") == 0) {
	  argv[i] = NULL;
	  cmd = CMD_TEST;
	}
	else if (strcmp(arg, "-i") == 0 || strcmp(arg, "--info") == 0) {
	  argv[i] = NULL;
	  cmd = CMD_INFO;
	}
	else if (strcmp(arg, "-p") == 0 || strcmp(arg, "--plugin") == 0) {
	  argv[i] = NULL;
	  if (++i < argc) {
		plugin_path = argv[i];
		argv[i] = NULL;
	  }
	}
	else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--connection") == 0) {
	  argv[i] = NULL;
	  if (++i < argc) {
		connection_path = argv[i];
		argv[i] = NULL;
	  }
	}
  }

  for (int i = 1, j = 1, n = argc; i < n; i++) {
	if (argv[i])
	  argv[j++] = argv[i];
	else
	  --argc;
  }

  void *handles[10] = { NULL, };
  int n_handles = 0;
  if (plugin_path == NULL)
	cmd = CMD_HELP;
  else {
	void *handle;
	const char *error;
#if defined(__sun)
	 
	const char SunStudio_libCrun[] = "libCrun.so.1";
	D(bug("  trying to open SunStudio C++ runtime '%s'\n", SunStudio_libCrun));
	if ((handle = dlopen(SunStudio_libCrun, RTLD_LAZY|RTLD_GLOBAL)) == NULL) {
	  npw_printf("ERROR: %s\n", dlerror());
	  return 1;
	}
	handles[n_handles++] = handle;
	dlerror();
#endif
	D(bug("  %s\n", plugin_path));
	if ((handle = dlopen(plugin_path, RTLD_LAZY)) == NULL) {
	  npw_printf("ERROR: %s\n", dlerror());
	  return 1;
	}
	handles[n_handles++] = handle;
	dlerror();
	g_plugin_NP_GetMIMEDescription = (NP_GetMIMEDescriptionUPP)dlsym(handle, "NP_GetMIMEDescription");
	if ((error = dlerror()) != NULL) {
	  npw_printf("ERROR: %s\n", error);
	  return 1;
	}
	g_plugin_NP_Initialize = (NP_InitializeUPP)dlsym(handle, "NP_Initialize");
	if ((error = dlerror()) != NULL) {
	  npw_printf("ERROR: %s\n", error);
	  return 1;
	}
	g_plugin_NP_Shutdown = (NP_ShutdownUPP)dlsym(handle, "NP_Shutdown");
	if ((error = dlerror()) != NULL) {
	  npw_printf("ERROR: %s\n", error);
	  return 1;
	}
	g_plugin_NP_GetValue = (NP_GetValueUPP)dlsym(handle, "NP_GetValue");
  }

  int ret = 1;
  switch (cmd) {
  case CMD_RUN:
	ret = do_main(argc, argv, connection_path);
	break;
  case CMD_TEST:
	ret = do_test();
	break;
  case CMD_INFO:
	ret = do_info();
	break;
  case CMD_HELP:
	ret = do_help(argv[0]);
	break;
  }

  while (--n_handles >= 0) {
	void * const handle = handles[n_handles];
	if (handle)
	  dlclose(handle);
  }
  return ret;
}
