void GLES2DecoderImpl::DoVertexAttribI4ui(
    GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  GLuint v[4] = { v0, v1, v2, v3 };
  if (SetVertexAttribValue("glVertexAttribI4ui", index, v)) {
    state_.SetGenericVertexAttribBaseType(
        index, SHADER_VARIABLE_UINT);
    api()->glVertexAttribI4uiFn(index, v0, v1, v2, v3);
  }
}
