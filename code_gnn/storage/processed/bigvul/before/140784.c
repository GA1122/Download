error::Error GLES2DecoderImpl::HandleDeleteShader(uint32 immediate_data_size,
                                                  const void* cmd_data) {
  const gles2::cmds::DeleteShader& c =
      *static_cast<const gles2::cmds::DeleteShader*>(cmd_data);
  GLuint client_id = c.shader;
  if (client_id) {
    Shader* shader = GetShader(client_id);
    if (shader) {
      if (!shader->IsDeleted()) {
        shader_manager()->Delete(shader);
      }
    } else {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glDeleteShader", "unknown shader");
    }
  }
  return error::kNoError;
}
