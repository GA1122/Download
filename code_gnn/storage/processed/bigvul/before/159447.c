void WebGL2RenderingContextBase::deleteFramebuffer(
    WebGLFramebuffer* framebuffer) {
  if (framebuffer && framebuffer->Opaque()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "deleteFramebuffer",
                      "cannot delete an opaque framebuffer");
    return;
  }
  if (!DeleteObject(framebuffer))
    return;
  GLenum target = 0;
  if (framebuffer == framebuffer_binding_) {
    if (framebuffer == read_framebuffer_binding_) {
      target = GL_FRAMEBUFFER;
      framebuffer_binding_ = nullptr;
      read_framebuffer_binding_ = nullptr;
    } else {
      target = GL_DRAW_FRAMEBUFFER;
      framebuffer_binding_ = nullptr;
    }
  } else if (framebuffer == read_framebuffer_binding_) {
    target = GL_READ_FRAMEBUFFER;
    read_framebuffer_binding_ = nullptr;
  }
  if (target) {
    GetDrawingBuffer()->Bind(target);
  }
}
