GLenum Framebuffer::IsPossiblyComplete() const {
  if (attachments_.empty()) {
    return GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
  }

  GLsizei width = -1;
  GLsizei height = -1;
  for (AttachmentMap::const_iterator it = attachments_.begin();
       it != attachments_.end(); ++it) {
    GLenum attachment_type = it->first;
    Attachment* attachment = it->second.get();
    if (!attachment->ValidForAttachmentType(attachment_type,
                                            manager_->max_color_attachments_)) {
      return GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
    }
    if (width < 0) {
      width = attachment->width();
      height = attachment->height();
      if (width == 0 || height == 0) {
        return GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
      }
    } else {
      if (attachment->width() != width || attachment->height() != height) {
        return GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT;
      }
    }

    if (!attachment->CanRenderTo()) {
      return GL_FRAMEBUFFER_UNSUPPORTED;
    }
  }

  return GL_FRAMEBUFFER_COMPLETE;
}
