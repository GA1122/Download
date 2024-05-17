void WebPluginProxy::SetDeferResourceLoading(unsigned long resource_id,
                                             bool defer) {
  Send(new PluginHostMsg_DeferResourceLoading(route_id_, resource_id, defer));
}
