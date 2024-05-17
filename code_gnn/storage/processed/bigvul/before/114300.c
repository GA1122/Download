void WebGraphicsContext3DCommandBufferImpl::deleteFramebuffer(
    WebGLId framebuffer) {
  gl_->DeleteFramebuffers(1, &framebuffer);
}
