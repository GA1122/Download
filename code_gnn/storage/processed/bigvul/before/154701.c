error::Error GLES2DecoderPassthroughImpl::DoGetRenderbufferParameteriv(
    GLenum target,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLint* params) {
  api()->glGetRenderbufferParameterivRobustANGLEFn(target, pname, bufsize,
                                                   length, params);
  return error::kNoError;
}
