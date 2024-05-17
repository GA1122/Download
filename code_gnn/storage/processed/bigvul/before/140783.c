error::Error GLES2DecoderImpl::HandleDeleteProgram(uint32 immediate_data_size,
                                                   const void* cmd_data) {
  const gles2::cmds::DeleteProgram& c =
      *static_cast<const gles2::cmds::DeleteProgram*>(cmd_data);
  GLuint client_id = c.program;
  if (client_id) {
    Program* program = GetProgram(client_id);
    if (program) {
      if (!program->IsDeleted()) {
        program_manager()->MarkAsDeleted(shader_manager(), program);
      }
    } else {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glDeleteProgram", "unknown program");
    }
  }
  return error::kNoError;
}
