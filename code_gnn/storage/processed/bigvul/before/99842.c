void WebPluginProxy::SetWindowlessBuffer(
    const TransportDIB::Handle& windowless_buffer,
    const TransportDIB::Handle& background_buffer) {
  windowless_dib_.reset(TransportDIB::Map(windowless_buffer));
  background_dib_.reset(TransportDIB::Map(background_buffer));
  windowless_context_.reset(CGBitmapContextCreate(
      windowless_dib_->memory(),
      delegate_->GetRect().width(),
      delegate_->GetRect().height(),
      8, 4 * delegate_->GetRect().width(),
      mac_util::GetSystemColorSpace(),
      kCGImageAlphaPremultipliedFirst |
      kCGBitmapByteOrder32Host));
  CGContextTranslateCTM(windowless_context_, 0, delegate_->GetRect().height());
  CGContextScaleCTM(windowless_context_, 1, -1);
  if (background_dib_.get()) {
    background_context_.reset(CGBitmapContextCreate(
        background_dib_->memory(),
        delegate_->GetRect().width(),
        delegate_->GetRect().height(),
        8, 4 * delegate_->GetRect().width(),
        mac_util::GetSystemColorSpace(),
        kCGImageAlphaPremultipliedFirst |
        kCGBitmapByteOrder32Host));
    CGContextTranslateCTM(background_context_, 0,
                          delegate_->GetRect().height());
    CGContextScaleCTM(background_context_, 1, -1);
  }

  static_cast<WebPluginDelegateImpl*>(delegate_)->UpdateContext(
      windowless_context_);
}
