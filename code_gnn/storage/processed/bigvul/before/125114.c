PluginProcessHost* PluginServiceImpl::FindNpapiPluginProcess(
    const FilePath& plugin_path) {
  for (PluginProcessHostIterator iter; !iter.Done(); ++iter) {
    if (iter->info().path == plugin_path)
      return *iter;
  }

  return NULL;
}
