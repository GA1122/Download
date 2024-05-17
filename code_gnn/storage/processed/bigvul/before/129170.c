void Framebuffer::MarkAttachmentsAsCleared(
      RenderbufferManager* renderbuffer_manager,
      TextureManager* texture_manager,
      bool cleared) {
  for (AttachmentMap::iterator it = attachments_.begin();
       it != attachments_.end(); ++it) {
    Attachment* attachment = it->second.get();
    if (attachment->cleared() != cleared) {
      attachment->SetCleared(renderbuffer_manager, texture_manager, cleared);
    }
  }
}
