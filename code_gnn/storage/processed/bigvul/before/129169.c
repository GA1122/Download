 void Framebuffer::MarkAttachmentAsCleared(
       RenderbufferManager* renderbuffer_manager,
       TextureManager* texture_manager,
      GLenum attachment,
      bool cleared) {
  AttachmentMap::iterator it = attachments_.find(attachment);
  if (it != attachments_.end()) {
    Attachment* a = it->second.get();
    if (a->cleared() != cleared) {
      a->SetCleared(renderbuffer_manager,
                    texture_manager,
                    cleared);
    }
  }
}
