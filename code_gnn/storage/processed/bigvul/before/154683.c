error::Error GLES2DecoderPassthroughImpl::DoGetFloatv(GLenum pname,
                                                      GLsizei bufsize,
                                                      GLsizei* length,
                                                      GLfloat* params) {
  return GetNumericHelper(
      pname, bufsize, length, params,
      [this](GLenum pname, GLsizei bufsize, GLsizei* length, GLfloat* params) {
        api()->glGetFloatvRobustANGLEFn(pname, bufsize, length, params);
      });
}
