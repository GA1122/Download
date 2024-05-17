static NPError g_NPP_New(NPMIMEType plugin_type, uint32_t instance_id,
						 uint16_t mode, int16_t argc, char *argn[], char *argv[],
						 NPSavedData *saved)
{
  PluginInstance *plugin = npw_plugin_instance_new(&PluginInstanceClass);
  if (plugin == NULL)
	return NPERR_OUT_OF_MEMORY_ERROR;
  plugin->instance_id = instance_id;
  id_link(instance_id, plugin);

  NPP instance = malloc(sizeof(*instance));
  if (instance == NULL)
	return NPERR_OUT_OF_MEMORY_ERROR;
  memset(instance, 0, sizeof(*instance));
  instance->ndata = plugin;
  plugin->instance = instance;

  for (int i = 0; i < argc; i++) {
	if (argn[i] == NULL)
	  continue;
	if (strcasecmp(argn[i], "width") == 0) {
	  if (i < argc && argv[i])
		plugin->width = atoi(argv[i]);
	}
	else if (strcasecmp(argn[i], "height") == 0) {
	  if (i < argc && argv[i])
		plugin->height = atoi(argv[i]);
	}
  }

  D(bugiI("NPP_New instance=%p\n", instance));
  NPError ret = plugin_funcs.newp(plugin_type, instance, mode, argc, argn, argv, saved);
  D(bugiD("NPP_New return: %d [%s]\n", ret, string_of_NPError(ret)));

  long supports_XEmbed = FALSE;
  if (mozilla_funcs.getvalue) {
	NPError error = mozilla_funcs.getvalue(NULL, NPNVSupportsXEmbedBool, (void *)&supports_XEmbed);
	if (error == NPERR_NO_ERROR && plugin_funcs.getvalue) {
	  long needs_XEmbed = FALSE;
	  error = plugin_funcs.getvalue(instance, NPPVpluginNeedsXEmbed, (void *)&needs_XEmbed);
	  if (error == NPERR_NO_ERROR)
		plugin->use_xembed = supports_XEmbed && needs_XEmbed;
	}
  }

  if (!plugin->use_xembed) {
	if (xt_source_create() < 0)
	  return NPERR_GENERIC_ERROR;
  }

  return ret;
}