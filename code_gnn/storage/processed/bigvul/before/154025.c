void GLES2DecoderImpl::DoGetVertexAttribIuiv(GLuint index,
                                             GLenum pname,
                                             GLuint* params,
                                             GLsizei params_size) {
  DoGetVertexAttribImpl<GLuint>(index, pname, params);
}
