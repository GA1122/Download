void WebPluginProxy::URLRedirectResponse(bool allow, int resource_id) {
  Send(new PluginHostMsg_URLRedirectResponse(route_id_, allow, resource_id));
}
