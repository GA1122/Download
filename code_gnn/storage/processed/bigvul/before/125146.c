void PluginServiceImpl::RegisterFilePathWatcher(FilePathWatcher* watcher,
                                                const FilePath& path) {
  bool result = watcher->Watch(path, false,
                               base::Bind(&NotifyPluginDirChanged));
  DCHECK(result);
}
