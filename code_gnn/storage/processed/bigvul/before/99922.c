void NPN_PushPopupsEnabledState(NPP id, NPBool enabled) {
  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin)
    plugin->PushPopupsEnabledState(enabled);
}
