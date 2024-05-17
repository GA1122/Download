void GLES2DecoderImpl::DoGetVertexAttribIiv(GLuint index,
                                            GLenum pname,
                                            GLint* params,
                                            GLsizei params_size) {
  DoGetVertexAttribImpl<GLint>(index, pname, params);
}
