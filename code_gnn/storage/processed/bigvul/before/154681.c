error::Error GLES2DecoderPassthroughImpl::DoGetBufferParameteriv(
    GLenum target,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLint* params) {
  CheckErrorCallbackState();
  api()->glGetBufferParameterivRobustANGLEFn(target, pname, bufsize, length,
                                             params);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }
  PatchGetBufferResults(target, pname, bufsize, length, params);
  return error::kNoError;
}
