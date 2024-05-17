void WebPluginDelegateProxy::OnAcceleratedSurfaceFreeTransportDIB(
    TransportDIB::Id dib_id) {
  if (render_view_)
    render_view_->AcceleratedSurfaceFreeTransportDIB(dib_id);
}
