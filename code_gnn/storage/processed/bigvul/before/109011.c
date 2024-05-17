void RenderViewImpl::PluginFocusChanged(bool focused, int plugin_id) {
  Send(new ViewHostMsg_PluginFocusChanged(routing_id(), focused, plugin_id));
}
