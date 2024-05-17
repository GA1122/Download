void WebPluginProxy::FocusChanged(bool focused) {
  IPC::Message* msg = new PluginHostMsg_FocusChanged(route_id_, focused);
  Send(msg);
}
