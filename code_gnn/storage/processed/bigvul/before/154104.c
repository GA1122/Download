void GLES2DecoderImpl::DoVertexAttrib3f(
    GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
  GLfloat v[4] = { v0, v1, v2, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib3f", index, v)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib3fFn(index, v0, v1, v2);
  }
}
