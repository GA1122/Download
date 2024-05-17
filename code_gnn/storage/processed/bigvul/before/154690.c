error::Error GLES2DecoderPassthroughImpl::DoGetIntegerv(GLenum pname,
                                                        GLsizei bufsize,
                                                        GLsizei* length,
                                                        GLint* params) {
  return GetNumericHelper(
      pname, bufsize, length, params,
      [this](GLenum pname, GLsizei bufsize, GLsizei* length, GLint* params) {
        api()->glGetIntegervRobustANGLEFn(pname, bufsize, length, params);
      });
}
