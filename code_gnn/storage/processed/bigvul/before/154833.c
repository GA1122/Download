error::Error GLES2DecoderPassthroughImpl::DoUniform1ui(GLint location,
                                                       GLuint x) {
  api()->glUniform1uiFn(location, x);
  return error::kNoError;
}
