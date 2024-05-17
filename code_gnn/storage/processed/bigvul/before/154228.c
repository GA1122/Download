error::Error GLES2DecoderImpl::HandleGetAttachedShaders(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetAttachedShaders& c =
      *static_cast<const volatile gles2::cmds::GetAttachedShaders*>(cmd_data);
  uint32_t result_size = c.result_size;
  GLuint program_id = static_cast<GLuint>(c.program);
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetAttachedShaders");
  if (!program) {
    return error::kNoError;
  }
  typedef cmds::GetAttachedShaders::Result Result;
  uint32_t max_count = Result::ComputeMaxResults(result_size);
  uint32_t checked_size = 0;
  if (!Result::ComputeSize(max_count).AssignIfValid(&checked_size)) {
    return error::kOutOfBounds;
  }
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, checked_size);
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  GLsizei count = 0;
  api()->glGetAttachedShadersFn(program->service_id(), max_count, &count,
                                result->GetData());
  for (GLsizei ii = 0; ii < count; ++ii) {
    if (!shader_manager()->GetClientId(result->GetData()[ii],
                                       &result->GetData()[ii])) {
      NOTREACHED();
      return error::kGenericError;
    }
  }
  result->SetNumResults(count);
  return error::kNoError;
}
