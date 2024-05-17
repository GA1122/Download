void Framebuffer::UnbindTexture(
    GLenum target, TextureRef* texture_ref) {
  bool done;
  do {
    done = true;
    for (AttachmentMap::const_iterator it = attachments_.begin();
         it != attachments_.end(); ++it) {
      Attachment* attachment = it->second.get();
      if (attachment->IsTexture(texture_ref)) {
        AttachTexture(it->first, NULL, GL_TEXTURE_2D, 0, 0);
        done = false;
        break;
      }
    }
  } while (!done);
}
