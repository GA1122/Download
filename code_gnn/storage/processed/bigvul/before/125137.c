void NotifyPluginDirChanged(const FilePath& path, bool error) {
  if (error) {
    NOTREACHED();
    return;
  }
  VLOG(1) << "Watched path changed: " << path.value();
  webkit::npapi::PluginList::Singleton()->RefreshPlugins();
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&PluginService::PurgePluginListCache,
                 static_cast<BrowserContext*>(NULL), false));
}
