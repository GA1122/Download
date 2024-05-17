static NPAPI::PluginInstance* FindInstance(NPP id) {
  if (id == NULL) {
    NOTREACHED();
    return NULL;
  }
  return reinterpret_cast<NPAPI::PluginInstance*>(id->ndata);
}
