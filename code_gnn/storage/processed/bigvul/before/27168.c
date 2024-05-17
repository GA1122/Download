static void plugin_instance_invalidate(PluginInstance *plugin)
{
  destroy_window(plugin);

   
  if (plugin->instance_id) {
	id_remove(plugin->instance_id);
	plugin->instance_id = 0;
  }
}
