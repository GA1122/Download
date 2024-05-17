NPError NPN_DestroyStream(NPP id, NPStream* stream, NPReason reason) {

  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin.get() == NULL) {
    NOTREACHED();
    return NPERR_GENERIC_ERROR;
  }

  return plugin->NPP_DestroyStream(stream, reason);
}
