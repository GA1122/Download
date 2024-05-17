error::Error GLES2DecoderPassthroughImpl::DoUniform2ui(GLint location,
                                                       GLuint x,
                                                       GLuint y) {
  api()->glUniform2uiFn(location, x, y);
  return error::kNoError;
}
