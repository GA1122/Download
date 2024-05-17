NPError NPN_PopUpContextMenu(NPP id, NPMenu* menu) {
  if (!menu)
    return NPERR_INVALID_PARAM;

  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin.get()) {
    return plugin->PopUpContextMenu(menu);
  }
  NOTREACHED();
  return NPERR_GENERIC_ERROR;
}
