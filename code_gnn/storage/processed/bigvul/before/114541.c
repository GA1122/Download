void WebPluginProxy::FreeSurfaceDIB(TransportDIB::Id dib_id) {
  Send(new PluginHostMsg_FreeTransportDIB(route_id_, dib_id));
}
