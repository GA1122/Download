void Framebuffer::UnbindRenderbuffer(
    GLenum target, Renderbuffer* renderbuffer) {
  bool done;
  do {
    done = true;
    for (AttachmentMap::const_iterator it = attachments_.begin();
         it != attachments_.end(); ++it) {
      Attachment* attachment = it->second.get();
      if (attachment->IsRenderbuffer(renderbuffer)) {
        AttachRenderbuffer(it->first, NULL);
        done = false;
        break;
      }
    }
  } while (!done);
}
