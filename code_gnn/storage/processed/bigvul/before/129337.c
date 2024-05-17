void GLES2DecoderImpl::DoVertexAttrib1fv(GLuint index, const GLfloat* v) {
  GLfloat t[4] = { v[0], 0.0f, 0.0f, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib1fv", index, t)) {
    glVertexAttrib1fv(index, v);
  }
}
