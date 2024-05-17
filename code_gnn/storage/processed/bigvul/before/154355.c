ScopedRenderBufferBinder::ScopedRenderBufferBinder(ContextState* state,
                                                   ErrorState* error_state,
                                                   GLuint id)
    : state_(state), error_state_(error_state) {
  ScopedGLErrorSuppressor suppressor("ScopedRenderBufferBinder::ctor",
                                     error_state_);
  state->api()->glBindRenderbufferEXTFn(GL_RENDERBUFFER, id);
}
