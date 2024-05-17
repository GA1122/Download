void WebPluginDelegateProxy::OnAcceleratedSurfaceSetTransportDIB(
    gfx::PluginWindowHandle window,
    int32 width,
    int32 height,
    TransportDIB::Handle transport_dib) {
  if (render_view_)
    render_view_->AcceleratedSurfaceSetTransportDIB(window, width, height,
                                                    transport_dib);
}
