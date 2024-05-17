void GLES2DecoderImpl::DoVertexAttrib2fv(GLuint index, const GLfloat* v) {
  GLfloat t[4] = { v[0], v[1], 0.0f, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib2fv", index, t)) {
    glVertexAttrib2fv(index, v);
  }
}
