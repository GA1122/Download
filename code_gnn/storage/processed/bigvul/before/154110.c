void GLES2DecoderImpl::DoVertexAttribI4uiv(GLuint index,
                                           const volatile GLuint* v) {
  GLuint t[4] = {v[0], v[1], v[2], v[3]};
  if (SetVertexAttribValue("glVertexAttribI4uiv", index, t)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_UINT);
    api()->glVertexAttribI4uivFn(index, t);
  }
}
