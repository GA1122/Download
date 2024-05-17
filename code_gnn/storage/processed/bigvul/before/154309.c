void GLES2DecoderImpl::InvalidateFramebufferImpl(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    const char* function_name,
    FramebufferOperation op) {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);

  bool has_depth_stencil_format = framebuffer &&
      framebuffer->HasDepthStencilFormatAttachment();
  bool invalidate_depth = false;
  bool invalidate_stencil = false;
  std::unique_ptr<GLenum[]> validated_attachments(new GLenum[count+1]);
  GLsizei validated_count = 0;

  GLenum thresh0 = GL_COLOR_ATTACHMENT0 + group_->max_color_attachments();
  GLenum thresh1 = GL_COLOR_ATTACHMENT15;
  for (GLsizei i = 0; i < count; ++i) {
    GLenum attachment = attachments[i];
    if (framebuffer) {
      if (attachment >= thresh0 && attachment <= thresh1) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name, "invalid attachment");
        return;
      }
      if (!validators_->attachment.IsValid(attachment)) {
        LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, attachment,
                                        "attachments");
        return;
      }
      if (has_depth_stencil_format) {
        switch (attachment) {
          case GL_DEPTH_ATTACHMENT:
            invalidate_depth = true;
            continue;
          case GL_STENCIL_ATTACHMENT:
            invalidate_stencil = true;
            continue;
          case GL_DEPTH_STENCIL_ATTACHMENT:
            invalidate_depth = true;
            invalidate_stencil = true;
            continue;
        }
      }
    } else {
      if (!validators_->backbuffer_attachment.IsValid(attachment)) {
        LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, attachment,
                                        "attachments");
        return;
      }
    }
    validated_attachments[validated_count++] = attachment;
  }
  if (invalidate_depth && invalidate_stencil) {
    validated_attachments[validated_count++] = GL_DEPTH_ATTACHMENT;
    validated_attachments[validated_count++] = GL_STENCIL_ATTACHMENT;
  }

  std::unique_ptr<GLenum[]> translated_attachments(new GLenum[validated_count]);
  for (GLsizei i = 0; i < validated_count; ++i) {
    GLenum attachment = validated_attachments[i];
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

  bool dirty = false;
  switch (op) {
    case kFramebufferDiscard:
      if (gl_version_info().is_es3) {
        api()->glInvalidateFramebufferFn(target, validated_count,
                                         translated_attachments.get());
      } else {
        api()->glDiscardFramebufferEXTFn(target, validated_count,
                                         translated_attachments.get());
      }
      dirty = true;
      break;
    case kFramebufferInvalidate:
      if (gl_version_info().IsLowerThanGL(4, 3)) {
      } else {
        api()->glInvalidateFramebufferFn(target, validated_count,
                                         translated_attachments.get());
        dirty = true;
      }
      break;
    case kFramebufferInvalidateSub:
      break;
  }

  if (!dirty)
    return;

  for (GLsizei i = 0; i < validated_count; ++i) {
    if (framebuffer) {
      if (validated_attachments[i] == GL_DEPTH_STENCIL_ATTACHMENT) {
        framebuffer->MarkAttachmentAsCleared(renderbuffer_manager(),
                                             texture_manager(),
                                             GL_DEPTH_ATTACHMENT,
                                             false);
        framebuffer->MarkAttachmentAsCleared(renderbuffer_manager(),
                                             texture_manager(),
                                             GL_STENCIL_ATTACHMENT,
                                             false);
      } else {
        framebuffer->MarkAttachmentAsCleared(renderbuffer_manager(),
                                             texture_manager(),
                                             validated_attachments[i],
                                             false);
      }
    } else {
      switch (validated_attachments[i]) {
        case GL_COLOR_EXT:
          backbuffer_needs_clear_bits_ |= GL_COLOR_BUFFER_BIT;
          break;
        case GL_DEPTH_EXT:
          backbuffer_needs_clear_bits_ |= GL_DEPTH_BUFFER_BIT;
          break;
        case GL_STENCIL_EXT:
          backbuffer_needs_clear_bits_ |= GL_STENCIL_BUFFER_BIT;
          break;
        default:
          NOTREACHED();
          break;
      }
    }
  }
}
