void NPN_InvalidateRegion(NPP id, NPRegion invalidRegion) {

  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  DCHECK(plugin.get() != NULL);
  if (plugin.get() && plugin->webplugin())
    plugin->webplugin()->Invalidate();
}
