void PluginServiceImpl::ForcePluginShutdown(const FilePath& plugin_path) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::IO)) {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(&PluginServiceImpl::ForcePluginShutdown,
                   base::Unretained(this), plugin_path));
    return;
  }

  PluginProcessHost* plugin = FindNpapiPluginProcess(plugin_path);
  if (plugin)
    plugin->ForceShutdown();
}
