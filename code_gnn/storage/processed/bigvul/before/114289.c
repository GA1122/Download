void WebGraphicsContext3DCommandBufferImpl::bindFramebuffer(
    WGC3Denum target,
    WebGLId framebuffer) {
  gl_->BindFramebuffer(target, framebuffer);
  bound_fbo_ = framebuffer;
}
