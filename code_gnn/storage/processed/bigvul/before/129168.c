void Framebuffer::MarkAsDeleted() {
  deleted_ = true;
  while (!attachments_.empty()) {
    Attachment* attachment = attachments_.begin()->second.get();
    attachment->DetachFromFramebuffer(this);
    attachments_.erase(attachments_.begin());
  }
}
