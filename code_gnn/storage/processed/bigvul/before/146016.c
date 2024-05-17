void WebGL2RenderingContextBase::RestoreCurrentFramebuffer() {
  bindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_binding_.Get());
  bindFramebuffer(GL_READ_FRAMEBUFFER, read_framebuffer_binding_.Get());
}
