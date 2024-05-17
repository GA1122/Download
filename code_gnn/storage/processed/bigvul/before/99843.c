void WebPluginProxy::SetWindowlessBuffer(
    const TransportDIB::Handle& windowless_buffer,
    const TransportDIB::Handle& background_buffer) {
  int width = delegate_->GetRect().width();
  int height = delegate_->GetRect().height();
  windowless_dib_.reset(TransportDIB::Map(windowless_buffer));
  if (windowless_dib_.get()) {
    windowless_canvas_.reset(windowless_dib_->GetPlatformCanvas(width, height));
  } else {
    windowless_canvas_.reset();
  }
  background_dib_.reset(TransportDIB::Map(background_buffer));
  if (background_dib_.get()) {
    background_canvas_.reset(background_dib_->GetPlatformCanvas(width, height));
  } else {
    background_canvas_.reset();
  }
}
