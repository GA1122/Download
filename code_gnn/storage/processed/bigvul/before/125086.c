bool ChromePluginServiceFilter::CanLoadPlugin(int render_process_id,
                                              const FilePath& path) {
  if (!render_process_id)
    return true;

  base::AutoLock auto_lock(lock_);
  const ProcessDetails* details = GetProcess(render_process_id);
  if (!details)
    return false;

  if (details->authorized_plugins.find(path) ==
          details->authorized_plugins.end() &&
      details->authorized_plugins.find(FilePath()) ==
          details->authorized_plugins.end()) {
    return false;
  }

  return true;
}
