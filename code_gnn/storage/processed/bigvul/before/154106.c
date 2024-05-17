void GLES2DecoderImpl::DoVertexAttrib4f(
    GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  GLfloat v[4] = { v0, v1, v2, v3, };
  if (SetVertexAttribValue("glVertexAttrib4f", index, v)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib4fFn(index, v0, v1, v2, v3);
  }
}