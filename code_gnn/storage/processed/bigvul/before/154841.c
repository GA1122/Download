error::Error GLES2DecoderPassthroughImpl::DoUniform3f(GLint location,
                                                      GLfloat x,
                                                      GLfloat y,
                                                      GLfloat z) {
  api()->glUniform3fFn(location, x, y, z);
  return error::kNoError;
}
