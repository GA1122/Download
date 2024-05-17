error::Error GLES2DecoderPassthroughImpl::DoGetBooleanv(GLenum pname,
                                                        GLsizei bufsize,
                                                        GLsizei* length,
                                                        GLboolean* params) {
  return GetNumericHelper(pname, bufsize, length, params,
                          [this](GLenum pname, GLsizei bufsize, GLsizei* length,
                                 GLboolean* params) {
                            api()->glGetBooleanvRobustANGLEFn(pname, bufsize,
                                                              length, params);
                          });
}
