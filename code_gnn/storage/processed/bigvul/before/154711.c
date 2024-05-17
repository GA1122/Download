error::Error GLES2DecoderPassthroughImpl::DoGetTexParameteriv(GLenum target,
                                                              GLenum pname,
                                                              GLsizei bufsize,
                                                              GLsizei* length,
                                                              GLint* params) {
  api()->glGetTexParameterivRobustANGLEFn(target, pname, bufsize, length,
                                          params);
  return error::kNoError;
}
