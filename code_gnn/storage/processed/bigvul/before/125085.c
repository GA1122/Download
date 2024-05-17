void ChromePluginServiceFilter::AuthorizePlugin(int render_process_id,
                                                const FilePath& plugin_path) {
  base::AutoLock auto_lock(lock_);
  ProcessDetails* details = GetOrRegisterProcess(render_process_id);
  details->authorized_plugins.insert(plugin_path);
}
