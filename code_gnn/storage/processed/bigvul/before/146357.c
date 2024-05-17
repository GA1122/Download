void WebGLRenderingContextBase::RestoreCurrentFramebuffer() {
  bindFramebuffer(GL_FRAMEBUFFER, framebuffer_binding_.Get());
}
