void Framebuffer::AttachTexture(
    GLenum attachment, TextureRef* texture_ref, GLenum target,
    GLint level, GLsizei samples) {
  const Attachment* a = GetAttachment(attachment);
  if (a)
    a->DetachFromFramebuffer(this);
  if (texture_ref) {
    attachments_[attachment] = scoped_refptr<Attachment>(
        new TextureAttachment(texture_ref, target, level, samples));
    texture_ref->texture()->AttachToFramebuffer();
  } else {
    attachments_.erase(attachment);
  }
  framebuffer_complete_state_count_id_ = 0;
}
