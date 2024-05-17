void PluginServiceImpl::OpenChannelToPpapiPlugin(
    int render_process_id,
    const FilePath& plugin_path,
    const FilePath& profile_data_directory,
    PpapiPluginProcessHost::PluginClient* client) {
  PpapiPluginProcessHost* plugin_host = FindOrStartPpapiPluginProcess(
      render_process_id, plugin_path, profile_data_directory, client);
  if (plugin_host) {
    plugin_host->OpenChannelToPlugin(client);
  } else {
    client->OnPpapiChannelOpened(IPC::ChannelHandle(), base::kNullProcessId, 0);
  }
}
