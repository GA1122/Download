void GLES2DecoderImpl::DoVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1) {
  GLfloat v[4] = { v0, v1, 0.0f, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib2f", index, v)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib2fFn(index, v0, v1);
  }
}
