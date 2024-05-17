PpapiPluginProcessHost* PluginServiceImpl::FindOrStartPpapiBrokerProcess(
    int render_process_id,
    const FilePath& plugin_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (filter_ && !filter_->CanLoadPlugin(render_process_id, plugin_path))
    return NULL;

  PpapiPluginProcessHost* plugin_host = FindPpapiBrokerProcess(plugin_path);
  if (plugin_host)
    return plugin_host;

  PepperPluginInfo* info = GetRegisteredPpapiPluginInfo(plugin_path);
  if (!info)
    return NULL;


  return PpapiPluginProcessHost::CreateBrokerHost(*info);
}
