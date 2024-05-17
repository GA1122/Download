GLenum Framebuffer::GetColorAttachmentFormat() const {
  AttachmentMap::const_iterator it = attachments_.find(GL_COLOR_ATTACHMENT0);
  if (it == attachments_.end()) {
    return 0;
  }
  const Attachment* attachment = it->second.get();
  return attachment->internal_format();
}
