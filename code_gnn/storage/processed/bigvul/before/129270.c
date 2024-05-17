void GLES2DecoderImpl::DoDiscardFramebufferEXT(GLenum target,
                                               GLsizei numAttachments,
                                               const GLenum* attachments) {
  Framebuffer* framebuffer =
      GetFramebufferInfoForTarget(GL_FRAMEBUFFER);

  for (GLsizei i = 0; i < numAttachments; ++i) {
    if ((framebuffer &&
        !validators_->attachment.IsValid(attachments[i])) ||
       (!framebuffer &&
        !validators_->backbuffer_attachment.IsValid(attachments[i]))) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(
          "glDiscardFramebufferEXT", attachments[i], "attachments");
      return;
    }
  }

  for (GLsizei i = 0; i < numAttachments; ++i) {
    if (framebuffer) {
      framebuffer->MarkAttachmentAsCleared(renderbuffer_manager(),
                                           texture_manager(),
                                           attachments[i],
                                           false);
    } else {
      switch (attachments[i]) {
        case GL_COLOR_EXT:
          backbuffer_needs_clear_bits_ |= GL_COLOR_BUFFER_BIT;
          break;
        case GL_DEPTH_EXT:
          backbuffer_needs_clear_bits_ |= GL_DEPTH_BUFFER_BIT;
        case GL_STENCIL_EXT:
          backbuffer_needs_clear_bits_ |= GL_STENCIL_BUFFER_BIT;
          break;
        default:
          NOTREACHED();
          break;
      }
    }
  }

  scoped_ptr<GLenum[]> translated_attachments(new GLenum[numAttachments]);
  for (GLsizei i = 0; i < numAttachments; ++i) {
    GLenum attachment = attachments[i];
    if (!framebuffer && GetBackbufferServiceId()) {
      switch (attachment) {
        case GL_COLOR_EXT:
          attachment = GL_COLOR_ATTACHMENT0;
          break;
        case GL_DEPTH_EXT:
          attachment = GL_DEPTH_ATTACHMENT;
          break;
        case GL_STENCIL_EXT:
          attachment = GL_STENCIL_ATTACHMENT;
          break;
        default:
          NOTREACHED();
          return;
      }
    }
    translated_attachments[i] = attachment;
  }

  glDiscardFramebufferEXT(target, numAttachments, translated_attachments.get());
}
