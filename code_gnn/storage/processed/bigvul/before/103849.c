void RenderView::PluginFocusChanged(bool focused, int plugin_id) {
  IPC::Message* msg = new ViewHostMsg_PluginFocusChanged(routing_id(),
                                                         focused, plugin_id);
  Send(msg);
}
