void PluginServiceImpl::StartWatchingPlugins() {
#if defined(OS_WIN)
  if (hkcu_key_.Create(HKEY_CURRENT_USER,
                       webkit::npapi::kRegistryMozillaPlugins,
                       KEY_NOTIFY) == ERROR_SUCCESS) {
    if (hkcu_key_.StartWatching() == ERROR_SUCCESS) {
      hkcu_event_.reset(new base::WaitableEvent(hkcu_key_.watch_event()));
      base::WaitableEventWatcher::EventCallback callback =
            base::Bind(&PluginServiceImpl::OnWaitableEventSignaled,
                       base::Unretained(this));
      hkcu_watcher_.StartWatching(hkcu_event_.get(), callback);
    }
  }
  if (hklm_key_.Create(HKEY_LOCAL_MACHINE,
                       webkit::npapi::kRegistryMozillaPlugins,
                       KEY_NOTIFY) == ERROR_SUCCESS) {
    if (hklm_key_.StartWatching() == ERROR_SUCCESS) {
      hklm_event_.reset(new base::WaitableEvent(hklm_key_.watch_event()));
      base::WaitableEventWatcher::EventCallback callback =
            base::Bind(&PluginServiceImpl::OnWaitableEventSignaled,
                       base::Unretained(this));
      hklm_watcher_.StartWatching(hklm_event_.get(), callback);
    }
  }
#endif
#if defined(OS_POSIX) && !defined(OS_OPENBSD) && !defined(OS_ANDROID)

  std::vector<FilePath> plugin_dirs;
  plugin_list_->GetPluginDirectories(&plugin_dirs);

  for (size_t i = 0; i < plugin_dirs.size(); ++i) {
#if defined(OS_WIN)
    if (!plugin_dirs[i].IsAbsolute())
      continue;
#endif
    FilePathWatcher* watcher = new FilePathWatcher();
    VLOG(1) << "Watching for changes in: " << plugin_dirs[i].value();
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&PluginServiceImpl::RegisterFilePathWatcher, watcher,
                   plugin_dirs[i]));
    file_watchers_.push_back(watcher);
  }
#endif
}
