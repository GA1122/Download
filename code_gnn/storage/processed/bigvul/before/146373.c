void WebGLRenderingContextBase::SetFramebuffer(GLenum target,
                                               WebGLFramebuffer* buffer) {
  if (buffer)
    buffer->SetHasEverBeenBound();

  if (target == GL_FRAMEBUFFER || target == GL_DRAW_FRAMEBUFFER) {
    framebuffer_binding_ = buffer;
    ApplyStencilTest();
  }
  if (!buffer) {
    GetDrawingBuffer()->Bind(target);
  } else {
    ContextGL()->BindFramebuffer(target, buffer->Object());
  }
}
