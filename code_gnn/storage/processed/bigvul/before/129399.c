error::Error GLES2DecoderImpl::HandleDeleteShader(
    uint32 immediate_data_size, const cmds::DeleteShader& c) {
  GLuint client_id = c.shader;
  if (client_id) {
    Shader* shader = GetShader(client_id);
    if (shader) {
      if (!shader->IsDeleted()) {
        glDeleteShader(shader->service_id());
        shader_manager()->MarkAsDeleted(shader);
      }
    } else {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glDeleteShader", "unknown shader");
    }
  }
  return error::kNoError;
}
