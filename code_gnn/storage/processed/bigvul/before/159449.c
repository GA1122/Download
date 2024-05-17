void WebGL2RenderingContextBase::drawBuffers(const Vector<GLenum>& buffers) {
  if (isContextLost())
    return;

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());
  GLsizei n = buffers.size();
  const GLenum* bufs = buffers.data();
  for (GLsizei i = 0; i < n; ++i) {
    switch (bufs[i]) {
      case GL_NONE:
      case GL_BACK:
      case GL_COLOR_ATTACHMENT0:
        break;
      default:
        if (bufs[i] > GL_COLOR_ATTACHMENT0 &&
            bufs[i] < static_cast<GLenum>(GL_COLOR_ATTACHMENT0 +
                                          MaxColorAttachments()))
          break;
        SynthesizeGLError(GL_INVALID_ENUM, "drawBuffers", "invalid buffer");
        return;
    }
  }
  if (!framebuffer_binding_) {
    if (n != 1) {
      SynthesizeGLError(GL_INVALID_OPERATION, "drawBuffers",
                        "the number of buffers is not 1");
      return;
    }
    if (bufs[0] != GL_BACK && bufs[0] != GL_NONE) {
      SynthesizeGLError(GL_INVALID_OPERATION, "drawBuffers", "BACK or NONE");
      return;
    }
    GLenum value = (bufs[0] == GL_BACK) ? GL_COLOR_ATTACHMENT0 : GL_NONE;
    ContextGL()->DrawBuffersEXT(1, &value);
    SetBackDrawBuffer(bufs[0]);
  } else {
    if (n > MaxDrawBuffers()) {
      SynthesizeGLError(GL_INVALID_VALUE, "drawBuffers",
                        "more than max draw buffers");
      return;
    }
    for (GLsizei i = 0; i < n; ++i) {
      if (bufs[i] != GL_NONE &&
          bufs[i] != static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT + i)) {
        SynthesizeGLError(GL_INVALID_OPERATION, "drawBuffers",
                          "COLOR_ATTACHMENTi_EXT or NONE");
        return;
      }
    }
    framebuffer_binding_->DrawBuffers(buffers);
  }
}
