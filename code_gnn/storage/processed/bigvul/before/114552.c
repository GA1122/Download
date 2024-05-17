void WebPluginProxy::SetAcceleratedDIB(
    gfx::PluginWindowHandle window,
    const gfx::Size& size,
    const TransportDIB::Handle& dib_handle) {
  Send(new PluginHostMsg_AcceleratedSurfaceSetTransportDIB(
      route_id_, window, size.width(), size.height(), dib_handle));
}
