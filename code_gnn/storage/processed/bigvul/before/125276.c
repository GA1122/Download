void RenderViewImpl::PluginCrashed(const FilePath& plugin_path,
                                   base::ProcessId plugin_pid) {
  Send(new ViewHostMsg_CrashedPlugin(routing_id_, plugin_path, plugin_pid));
}