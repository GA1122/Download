void WebGLRenderingContextBase::ApplyStencilTest() {
  bool have_stencil_buffer = false;

  if (framebuffer_binding_) {
    have_stencil_buffer = framebuffer_binding_->HasStencilBuffer();
  } else {
    WebGLContextAttributes* attributes = getContextAttributes();
    have_stencil_buffer = attributes && attributes->stencil();
  }
  EnableOrDisable(GL_STENCIL_TEST, stencil_enabled_ && have_stencil_buffer);
}
