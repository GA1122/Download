void GLES2DecoderImpl::DoVertexAttrib1f(GLuint index, GLfloat v0) {
  GLfloat v[4] = { v0, 0.0f, 0.0f, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib1f", index, v)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib1fFn(index, v0);
  }
}
