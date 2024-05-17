void WebGL2RenderingContextBase::UpdateBuffersToAutoClear(
    WebGL2RenderingContextBase::ClearBufferCaller caller,
    GLenum buffer,
    GLint drawbuffer) {

  if (framebuffer_binding_)
    return;

  if (scissor_enabled_)
    return;

  if (drawbuffer != 0)
    return;


  switch (caller) {
    case kClearBufferiv:
      if (buffer != GL_STENCIL)
        return;
      break;
    case kClearBufferfv:
      if (buffer != GL_COLOR && buffer != GL_DEPTH)
        return;
      break;
    case kClearBufferuiv:
      return;
    case kClearBufferfi:
      if (buffer != GL_DEPTH_STENCIL)
        return;
      break;
  }

  GLbitfield buffers_to_clear = 0;

  switch (buffer) {
    case GL_COLOR:
      buffers_to_clear = GL_COLOR_BUFFER_BIT;
      break;
    case GL_DEPTH:
      buffers_to_clear = GL_DEPTH_BUFFER_BIT;
      break;
    case GL_STENCIL:
      buffers_to_clear = GL_STENCIL_BUFFER_BIT;
      break;
    case GL_DEPTH_STENCIL:
      buffers_to_clear = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
      break;
    default:
      return;
  }

  GetDrawingBuffer()->SetBuffersToAutoClear(
      GetDrawingBuffer()->GetBuffersToAutoClear() & (~buffers_to_clear));
}
