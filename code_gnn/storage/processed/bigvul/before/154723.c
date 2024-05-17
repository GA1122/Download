error::Error GLES2DecoderPassthroughImpl::DoGetVertexAttribIiv(GLuint index,
                                                               GLenum pname,
                                                               GLsizei bufsize,
                                                               GLsizei* length,
                                                               GLint* params) {
  api()->glGetVertexAttribIivRobustANGLEFn(index, pname, bufsize, length,
                                           params);
  return error::kNoError;
}
