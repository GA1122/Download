void Framebuffer::OnDidRenderTo() const {
  for (AttachmentMap::const_iterator it = attachments_.begin();
       it != attachments_.end(); ++it) {
    it->second->OnDidRenderTo();
  }
}
