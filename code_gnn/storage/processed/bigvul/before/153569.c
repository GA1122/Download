void GLES2Implementation::BindRenderbufferHelper(GLenum target,
                                                 GLuint renderbuffer) {
  bool changed = false;
  switch (target) {
    case GL_RENDERBUFFER:
      if (bound_renderbuffer_ != renderbuffer) {
        bound_renderbuffer_ = renderbuffer;
        changed = true;
      }
      break;
    default:
      changed = true;
      break;
  }
  if (changed) {
    GetIdHandler(SharedIdNamespaces::kRenderbuffers)
        ->MarkAsUsedForBind(this, target, renderbuffer,
                            &GLES2Implementation::BindRenderbufferStub);
  }
}
