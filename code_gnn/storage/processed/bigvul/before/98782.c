void WebPluginDelegateProxy::OnAcceleratedSurfaceAllocTransportDIB(
    size_t size,
    TransportDIB::Handle* dib_handle) {
  if (render_view_)
    *dib_handle = render_view_->AcceleratedSurfaceAllocTransportDIB(size);
  else
    *dib_handle = TransportDIB::DefaultHandleValue();
}
