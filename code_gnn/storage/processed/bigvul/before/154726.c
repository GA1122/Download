error::Error GLES2DecoderPassthroughImpl::DoGetVertexAttribfv(GLuint index,
                                                              GLenum pname,
                                                              GLsizei bufsize,
                                                              GLsizei* length,
                                                              GLfloat* params) {
  api()->glGetVertexAttribfvRobustANGLEFn(index, pname, bufsize, length,
                                          params);
  return error::kNoError;
}
