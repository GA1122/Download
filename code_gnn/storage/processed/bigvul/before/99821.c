void WebPluginProxy::CancelResource(unsigned long id) {
  Send(new PluginHostMsg_CancelResource(route_id_, id));
  resource_clients_.erase(id);
}
