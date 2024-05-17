bool RenderBuffer::AllocateStorage(const gfx::Size& size, GLenum format,
                                   GLsizei samples) {
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedRenderBufferBinder binder(decoder_, id_);
  if (samples <= 1) {
    glRenderbufferStorageEXT(GL_RENDERBUFFER,
                             format,
                             size.width(),
                             size.height());
  } else {
    if (GLES2Decoder::IsAngle()) {
      glRenderbufferStorageMultisampleANGLE(GL_RENDERBUFFER,
                                            samples,
                                            format,
                                            size.width(),
                                            size.height());
    } else {
      glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER,
                                          samples,
                                          format,
                                          size.width(),
                                          size.height());
    }
  }
  bool success = glGetError() == GL_NO_ERROR;
  if (success) {
    memory_tracker_.UpdateMemRepresented(
        size.width() * size.height() * samples *
        GLES2Util::RenderbufferBytesPerPixel(format));
    TRACE_BACKBUFFER_MEMORY_TOTAL(decoder_);
  }
  return success;
}
