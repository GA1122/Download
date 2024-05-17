void PluginServiceImpl::FinishOpenChannelToPlugin(
    int render_process_id,
    const FilePath& plugin_path,
    PluginProcessHost::Client* client) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (!ContainsKey(pending_plugin_clients_, client))
    return;
  pending_plugin_clients_.erase(client);

  PluginProcessHost* plugin_host = FindOrStartNpapiPluginProcess(
      render_process_id, plugin_path);
  if (plugin_host) {
    client->OnFoundPluginProcessHost(plugin_host);
    plugin_host->OpenChannelToPlugin(client);
  } else {
    client->OnError();
  }
}
