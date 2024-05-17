error::Error GLES2DecoderPassthroughImpl::DoUniform1f(GLint location,
                                                      GLfloat x) {
  api()->glUniform1fFn(location, x);
  return error::kNoError;
}
