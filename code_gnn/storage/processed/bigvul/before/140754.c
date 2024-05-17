void GLES2DecoderImpl::DoShaderSource(
    GLuint client_id, GLsizei count, const char** data, const GLint* length) {
  std::string str;
  for (GLsizei ii = 0; ii < count; ++ii) {
    if (length && length[ii] > 0)
      str.append(data[ii], length[ii]);
    else
      str.append(data[ii]);
  }
  Shader* shader = GetShaderInfoNotProgram(client_id, "glShaderSource");
  if (!shader) {
    return;
  }
  shader->set_source(str);
}
