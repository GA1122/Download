error::Error GLES2DecoderPassthroughImpl::DoGetVertexAttribIuiv(
    GLuint index,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLuint* params) {
  api()->glGetVertexAttribIuivRobustANGLEFn(index, pname, bufsize, length,
                                            params);
  return error::kNoError;
}
