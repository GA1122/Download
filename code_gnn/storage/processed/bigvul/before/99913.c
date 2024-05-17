void* NPN_MemAlloc(uint32 size) {
  scoped_refptr<NPAPI::PluginHost> host = NPAPI::PluginHost::Singleton();
  if (host != NULL) {
    return malloc(size);
  }
  return NULL;
}
