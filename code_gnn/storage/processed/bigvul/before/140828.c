error::Error GLES2DecoderImpl::HandleUniformBlockBinding(
    uint32_t immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::UniformBlockBinding& c =
      *static_cast<const gles2::cmds::UniformBlockBinding*>(cmd_data);
  GLuint client_id = c.program;
  GLuint index = static_cast<GLuint>(c.index);
  GLuint binding = static_cast<GLuint>(c.binding);
  Program* program = GetProgramInfoNotShader(
      client_id, "glUniformBlockBinding");
  if (!program) {
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  glUniformBlockBinding(service_id, index, binding);
  return error::kNoError;
}
