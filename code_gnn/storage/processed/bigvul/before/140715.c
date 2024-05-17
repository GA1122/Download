void GLES2DecoderImpl::DeleteValuebuffersCHROMIUMHelper(
    GLsizei n,
    const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    Valuebuffer* valuebuffer = GetValuebuffer(client_ids[ii]);
    if (valuebuffer) {
      if (state_.bound_valuebuffer.get() == valuebuffer) {
        state_.bound_valuebuffer = NULL;
      }
      RemoveValuebuffer(client_ids[ii]);
    }
  }
}
