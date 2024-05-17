error::Error GLES2DecoderPassthroughImpl::DoGetVertexAttribiv(GLuint index,
                                                              GLenum pname,
                                                              GLsizei bufsize,
                                                              GLsizei* length,
                                                              GLint* params) {
  api()->glGetVertexAttribivRobustANGLEFn(index, pname, bufsize, length,
                                          params);
  return error::kNoError;
}
