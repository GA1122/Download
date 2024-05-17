GLenum GLES2DecoderImpl::GetBoundReadFramebufferTextureType() {
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (framebuffer) {
    return framebuffer->GetReadBufferTextureType();
  } else {   
    if (back_buffer_read_buffer_ == GL_NONE)
      return 0;
    return GL_UNSIGNED_BYTE;
  }
}
