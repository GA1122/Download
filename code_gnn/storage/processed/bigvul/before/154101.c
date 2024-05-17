void GLES2DecoderImpl::DoVertexAttrib1fv(GLuint index,
                                         const volatile GLfloat* v) {
  GLfloat t[4] = { v[0], 0.0f, 0.0f, 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib1fv", index, t)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib1fvFn(index, t);
  }
}
