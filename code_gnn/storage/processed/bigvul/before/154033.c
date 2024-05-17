bool GLES2DecoderImpl::DoIsShader(GLuint client_id) {
  const Shader* shader = GetShader(client_id);
  return shader != nullptr && !shader->IsDeleted();
}
