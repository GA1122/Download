error::Error GLES2DecoderImpl::ShaderSourceHelper(
    GLuint client_id, const char* data, uint32 data_size) {
  std::string str(data, data + data_size);
  Shader* shader = GetShaderInfoNotProgram(client_id, "glShaderSource");
  if (!shader) {
    return error::kNoError;
  }
  shader->UpdateSource(str.c_str());
  return error::kNoError;
}
