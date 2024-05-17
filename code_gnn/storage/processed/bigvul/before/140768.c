bool GLES2DecoderImpl::GenValuebuffersCHROMIUMHelper(GLsizei n,
                                                     const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetValuebuffer(client_ids[ii])) {
      return false;
    }
  }
  for (GLsizei ii = 0; ii < n; ++ii) {
    CreateValuebuffer(client_ids[ii]);
  }
  return true;
}
