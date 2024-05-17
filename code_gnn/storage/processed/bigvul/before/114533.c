void WebPluginProxy::BindFakePluginWindowHandle(bool opaque) {
  Send(new PluginHostMsg_BindFakePluginWindowHandle(route_id_, opaque));
}
