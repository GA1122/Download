GLenum Framebuffer::GetStatus(
    TextureManager* texture_manager, GLenum target) const {
  std::string signature;
  if (allow_framebuffer_combo_complete_map_) {
    signature = base::StringPrintf("|FBO|target=%04x", target);
    for (AttachmentMap::const_iterator it = attachments_.begin();
         it != attachments_.end(); ++it) {
      Attachment* attachment = it->second.get();
      signature +=
          base::StringPrintf("|Attachment|attachmentpoint=%04x", it->first);
      attachment->AddToSignature(texture_manager, &signature);
    }

    if (!framebuffer_combo_complete_map_) {
      framebuffer_combo_complete_map_ = new FramebufferComboCompleteMap();
    }

    FramebufferComboCompleteMap::const_iterator it =
        framebuffer_combo_complete_map_->find(signature);
    if (it != framebuffer_combo_complete_map_->end()) {
      return GL_FRAMEBUFFER_COMPLETE;
    }
  }

  GLenum result = glCheckFramebufferStatusEXT(target);

  if (allow_framebuffer_combo_complete_map_ &&
      result == GL_FRAMEBUFFER_COMPLETE) {
    framebuffer_combo_complete_map_->insert(std::make_pair(signature, true));
  }

  return result;
}
