void GLES2DecoderImpl::DoVertexAttrib3fv(GLuint index,
                                         const volatile GLfloat* v) {
  GLfloat t[4] = { v[0], v[1], v[2], 1.0f, };
  if (SetVertexAttribValue("glVertexAttrib3fv", index, t)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_FLOAT);
    api()->glVertexAttrib3fvFn(index, t);
  }
}
