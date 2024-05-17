bool LoadPluginListInProcess() {
#if defined(OS_WIN)
  return true;
#else
  return !webkit::npapi::NPAPIPluginsSupported();
#endif
}
