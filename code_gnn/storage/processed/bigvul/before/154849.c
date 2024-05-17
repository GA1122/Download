error::Error GLES2DecoderPassthroughImpl::DoUniform4i(GLint location,
                                                      GLint x,
                                                      GLint y,
                                                      GLint z,
                                                      GLint w) {
  api()->glUniform4iFn(location, x, y, z, w);
  return error::kNoError;
}
