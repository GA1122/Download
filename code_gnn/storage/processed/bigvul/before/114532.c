void WebPluginProxy::AllocSurfaceDIB(const size_t size,
                                     TransportDIB::Handle* dib_handle) {
  Send(new PluginHostMsg_AllocTransportDIB(route_id_, size, dib_handle));
}
