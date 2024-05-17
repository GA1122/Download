void GLES2Implementation::BindFramebufferHelper(GLenum target,
                                                GLuint framebuffer) {
  bool changed = false;
  switch (target) {
    case GL_FRAMEBUFFER:
      if (bound_framebuffer_ != framebuffer ||
          bound_read_framebuffer_ != framebuffer) {
        bound_framebuffer_ = framebuffer;
        bound_read_framebuffer_ = framebuffer;
        changed = true;
      }
      break;
    case GL_READ_FRAMEBUFFER:
      DCHECK(capabilities_.major_version >= 3 ||
             IsChromiumFramebufferMultisampleAvailable());
      if (bound_read_framebuffer_ != framebuffer) {
        bound_read_framebuffer_ = framebuffer;
        changed = true;
      }
      break;
    case GL_DRAW_FRAMEBUFFER:
      DCHECK(capabilities_.major_version >= 3 ||
             IsChromiumFramebufferMultisampleAvailable());
      if (bound_framebuffer_ != framebuffer) {
        bound_framebuffer_ = framebuffer;
        changed = true;
      }
      break;
    default:
      SetGLErrorInvalidEnum("glBindFramebuffer", target, "target");
      return;
  }

  if (changed) {
    if (framebuffer != 0)
      GetIdAllocator(IdNamespaces::kFramebuffers)->MarkAsUsed(framebuffer);
    helper_->BindFramebuffer(target, framebuffer);
  }
}
