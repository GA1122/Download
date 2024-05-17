WebKit::WebString WebGraphicsContext3DCommandBufferImpl::getString(
    WGC3Denum name) {
  return WebKit::WebString::fromUTF8(
      reinterpret_cast<const char*>(gl_->GetString(name)));
}
