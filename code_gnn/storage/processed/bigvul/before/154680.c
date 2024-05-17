error::Error GLES2DecoderPassthroughImpl::DoGetBufferParameteri64v(
    GLenum target,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLint64* params) {
  CheckErrorCallbackState();
  api()->glGetBufferParameteri64vRobustANGLEFn(target, pname, bufsize, length,
                                               params);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }
  PatchGetBufferResults(target, pname, bufsize, length, params);
  return error::kNoError;
}
