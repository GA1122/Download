void WebPluginProxy::AcceleratedPluginEnabledRendering() {
  Send(new PluginHostMsg_AcceleratedPluginEnabledRendering(route_id_));
}
