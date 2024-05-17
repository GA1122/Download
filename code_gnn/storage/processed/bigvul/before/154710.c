error::Error GLES2DecoderPassthroughImpl::DoGetTexParameterfv(GLenum target,
                                                              GLenum pname,
                                                              GLsizei bufsize,
                                                              GLsizei* length,
                                                              GLfloat* params) {
  api()->glGetTexParameterfvRobustANGLEFn(target, pname, bufsize, length,
                                          params);
  return error::kNoError;
}
