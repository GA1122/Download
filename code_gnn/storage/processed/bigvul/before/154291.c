error::Error GLES2DecoderImpl::HandleUniformBlockBinding(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const char* func_name = "glUniformBlockBinding";
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::UniformBlockBinding& c =
      *static_cast<const volatile gles2::cmds::UniformBlockBinding*>(cmd_data);
  GLuint client_id = c.program;
  GLuint index = static_cast<GLuint>(c.index);
  GLuint binding = static_cast<GLuint>(c.binding);
  Program* program = GetProgramInfoNotShader(client_id, func_name);
  if (!program) {
    return error::kNoError;
  }
  if (index >= program->uniform_block_size_info().size()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name,
        "uniformBlockIndex is not an active uniform block index");
    return error::kNoError;
  }
  if (binding >= group_->max_uniform_buffer_bindings()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name,
        "uniformBlockBinding >= MAX_UNIFORM_BUFFER_BINDINGS");
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  api()->glUniformBlockBindingFn(service_id, index, binding);
  program->SetUniformBlockBinding(index, binding);
  return error::kNoError;
}
