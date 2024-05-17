void WebGL2RenderingContextBase::readBuffer(GLenum mode) {
  if (isContextLost())
    return;

  switch (mode) {
    case GL_BACK:
    case GL_NONE:
    case GL_COLOR_ATTACHMENT0:
      break;
    default:
      if (mode > GL_COLOR_ATTACHMENT0 &&
          mode <
              static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + MaxColorAttachments()))
        break;
      SynthesizeGLError(GL_INVALID_ENUM, "readBuffer", "invalid read buffer");
      return;
  }

  WebGLFramebuffer* read_framebuffer_binding =
      GetFramebufferBinding(GL_READ_FRAMEBUFFER);
  if (!read_framebuffer_binding) {
    DCHECK(GetDrawingBuffer());
    if (mode != GL_BACK && mode != GL_NONE) {
      SynthesizeGLError(GL_INVALID_OPERATION, "readBuffer",
                        "invalid read buffer");
      return;
    }
    read_buffer_of_default_framebuffer_ = mode;
    if (mode == GL_BACK)
      mode = GL_COLOR_ATTACHMENT0;
  } else {
    if (mode == GL_BACK) {
      SynthesizeGLError(GL_INVALID_OPERATION, "readBuffer",
                        "invalid read buffer");
      return;
    }
    read_framebuffer_binding->ReadBuffer(mode);
  }
  ContextGL()->ReadBuffer(mode);
}
