  TextureAttachment(
      TextureRef* texture_ref, GLenum target, GLint level, GLsizei samples)
      : texture_ref_(texture_ref),
        target_(target),
        level_(level),
        samples_(samples) {
  }
