void GLES2DecoderImpl::DoVertexAttrib4fv(GLuint index, const GLfloat* v) {
  if (SetVertexAttribValue("glVertexAttrib4fv", index, v)) {
    glVertexAttrib4fv(index, v);
  }
}
