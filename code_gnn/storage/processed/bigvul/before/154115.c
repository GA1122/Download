void GLES2DecoderImpl::EnsureRenderbufferBound() {
  if (!state_.bound_renderbuffer_valid) {
    state_.bound_renderbuffer_valid = true;
    api()->glBindRenderbufferEXTFn(GL_RENDERBUFFER,
                                   state_.bound_renderbuffer.get()
                                       ? state_.bound_renderbuffer->service_id()
                                       : 0);
  }
}
