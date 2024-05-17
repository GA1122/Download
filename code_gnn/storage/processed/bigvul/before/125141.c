void PluginServiceImpl::OpenChannelToPpapiBroker(
    int render_process_id,
    const FilePath& path,
    PpapiPluginProcessHost::BrokerClient* client) {
  PpapiPluginProcessHost* plugin_host = FindOrStartPpapiBrokerProcess(
      render_process_id, path);
  if (plugin_host) {
    plugin_host->OpenChannelToPlugin(client);
  } else {
    client->OnPpapiChannelOpened(IPC::ChannelHandle(), base::kNullProcessId, 0);
  }
}
