void WebPluginProxy::AcceleratedPluginSwappedIOSurface() {
  Send(new PluginHostMsg_AcceleratedPluginSwappedIOSurface(
      route_id_));
}
