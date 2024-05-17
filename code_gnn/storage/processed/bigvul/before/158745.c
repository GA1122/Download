ScopedRenderBufferBinder::ScopedRenderBufferBinder(ContextState* state,
                                                   GLuint id)
    : state_(state) {
  ScopedGLErrorSuppressor suppressor(
      "ScopedRenderBufferBinder::ctor", state_->GetErrorState());
  state->api()->glBindRenderbufferEXTFn(GL_RENDERBUFFER, id);
}
