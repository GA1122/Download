void ChromePluginServiceFilter::UnrestrictPlugin(
    const FilePath& plugin_path) {
  base::AutoLock auto_lock(lock_);
   restricted_plugins_.erase(plugin_path);
 }
