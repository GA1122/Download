GLenum GLES2DecoderImpl::GetBoundColorDrawBufferType(GLint drawbuffer_i) {
  DCHECK(drawbuffer_i >= 0 &&
         drawbuffer_i < static_cast<GLint>(group_->max_draw_buffers()));
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (!framebuffer) {
    return 0;
  }
  GLenum drawbuffer = static_cast<GLenum>(GL_DRAW_BUFFER0 + drawbuffer_i);
  if (framebuffer->GetDrawBuffer(drawbuffer) == GL_NONE) {
    return 0;
  }
  GLenum attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + drawbuffer_i);
  const Framebuffer::Attachment* buffer =
      framebuffer->GetAttachment(attachment);
  if (!buffer) {
    return 0;
  }
  return buffer->texture_type();
}
