error::Error GLES2DecoderPassthroughImpl::DoUniform3i(GLint location,
                                                      GLint x,
                                                      GLint y,
                                                      GLint z) {
  api()->glUniform3iFn(location, x, y, z);
  return error::kNoError;
}
