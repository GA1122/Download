error::Error GLES2DecoderPassthroughImpl::DoGetInteger64v(GLenum pname,
                                                          GLsizei bufsize,
                                                          GLsizei* length,
                                                          GLint64* params) {
  return GetNumericHelper(
      pname, bufsize, length, params,
      [this](GLenum pname, GLsizei bufsize, GLsizei* length, GLint64* params) {
        api()->glGetInteger64vRobustANGLEFn(pname, bufsize, length, params);
      });
}
