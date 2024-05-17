void NPN_PopPopupsEnabledState(NPP id) {
  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin)
    plugin->PopPopupsEnabledState();
}
