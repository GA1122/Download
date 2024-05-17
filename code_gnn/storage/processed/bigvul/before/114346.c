void WebGraphicsContext3DCommandBufferImpl::texParameteri(
    WGC3Denum target, WGC3Denum pname, WGC3Dint param) {
  if (pname == kTextureWrapR) {
    return;
  }
  gl_->TexParameteri(target, pname, param);
}
