bool Framebuffer::IsCleared() const {
  for (AttachmentMap::const_iterator it = attachments_.begin();
       it != attachments_.end(); ++it) {
    Attachment* attachment = it->second.get();
    if (!attachment->cleared()) {
      return false;
    }
  }
  return true;
}
