static NPError g_NPP_Destroy(NPP instance, NPSavedData **sdata)
{
  if (instance == NULL)
	return NPERR_INVALID_INSTANCE_ERROR;

  PluginInstance *plugin = PLUGIN_INSTANCE(instance);
  if (plugin == NULL)
	return NPERR_INVALID_INSTANCE_ERROR;

  if (sdata)
	*sdata = NULL;

  delayed_calls_process(plugin, TRUE);

  D(bugiI("NPP_Destroy instance=%p\n", instance));
  NPError ret = plugin_funcs.destroy(instance, sdata);
  D(bugiD("NPP_Destroy return: %d [%s]\n", ret, string_of_NPError(ret)));

  if (!plugin->use_xembed)
	xt_source_destroy();

  npw_plugin_instance_invalidate(plugin);
  npw_plugin_instance_unref(plugin);
  return ret;
}
