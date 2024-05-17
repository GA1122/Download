ScopedRenderBufferBinder::ScopedRenderBufferBinder(ContextState* state,
                                                   GLuint id)
    : state_(state) {
  ScopedGLErrorSuppressor suppressor(
      "ScopedRenderBufferBinder::ctor", state_->GetErrorState());
  glBindRenderbufferEXT(GL_RENDERBUFFER, id);
}
