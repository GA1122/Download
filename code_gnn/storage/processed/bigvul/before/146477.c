void WebGLRenderingContextBase::deleteFramebuffer(
    WebGLFramebuffer* framebuffer) {
  if (!DeleteObject(framebuffer))
    return;
  if (framebuffer == framebuffer_binding_) {
    framebuffer_binding_ = nullptr;
    GetDrawingBuffer()->Bind(GL_FRAMEBUFFER);
  }
}
