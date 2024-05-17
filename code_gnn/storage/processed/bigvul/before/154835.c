error::Error GLES2DecoderPassthroughImpl::DoUniform2f(GLint location,
                                                      GLfloat x,
                                                      GLfloat y) {
  api()->glUniform2fFn(location, x, y);
  return error::kNoError;
}
