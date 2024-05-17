void GLES2DecoderImpl::MarkDrawBufferAsCleared(
    GLenum buffer, GLint drawbuffer_i) {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (!framebuffer)
    return;
  GLenum attachment  = 0;
  switch (buffer) {
    case GL_COLOR:
      DCHECK(drawbuffer_i >= 0 &&
             drawbuffer_i < static_cast<GLint>(group_->max_draw_buffers()));
      attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + drawbuffer_i);
      break;
    case GL_DEPTH:
      attachment = GL_DEPTH_ATTACHMENT;
      break;
    case GL_STENCIL:
      attachment = GL_STENCIL_ATTACHMENT;
      break;
    default:
      NOTREACHED();
  }
  framebuffer->MarkAttachmentAsCleared(
      renderbuffer_manager(), texture_manager(), attachment, true);
}
