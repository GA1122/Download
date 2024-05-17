PpapiPluginProcessHost* PluginServiceImpl::FindPpapiPluginProcess(
    const FilePath& plugin_path,
    const FilePath& profile_data_directory) {
  for (PpapiPluginProcessHostIterator iter; !iter.Done(); ++iter) {
    if (iter->plugin_path() == plugin_path &&
        iter->profile_data_directory() == profile_data_directory) {
      return *iter;
    }
  }
  return NULL;
}
