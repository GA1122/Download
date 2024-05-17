void NPN_MemFree(void* ptr) {
  scoped_refptr<NPAPI::PluginHost> host = NPAPI::PluginHost::Singleton();
  if (host != NULL) {
    if (ptr != NULL && ptr != reinterpret_cast<void*>(-1))
      free(ptr);
  }
}
