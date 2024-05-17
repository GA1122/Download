error::Error GLES2DecoderPassthroughImpl::DoUniform4f(GLint location,
                                                      GLfloat x,
                                                      GLfloat y,
                                                      GLfloat z,
                                                      GLfloat w) {
  api()->glUniform4fFn(location, x, y, z, w);
  return error::kNoError;
}
