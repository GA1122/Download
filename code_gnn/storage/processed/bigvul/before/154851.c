error::Error GLES2DecoderPassthroughImpl::DoUniform4ui(GLint location,
                                                       GLuint x,
                                                       GLuint y,
                                                       GLuint z,
                                                       GLuint w) {
  api()->glUniform4uiFn(location, x, y, z, w);
  return error::kNoError;
}
