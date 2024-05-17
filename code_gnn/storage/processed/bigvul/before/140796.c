error::Error GLES2DecoderImpl::HandleGetAttachedShaders(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::GetAttachedShaders& c =
      *static_cast<const gles2::cmds::GetAttachedShaders*>(cmd_data);
  uint32 result_size = c.result_size;
  GLuint program_id = static_cast<GLuint>(c.program);
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetAttachedShaders");
  if (!program) {
    return error::kNoError;
  }
  typedef cmds::GetAttachedShaders::Result Result;
  uint32 max_count = Result::ComputeMaxResults(result_size);
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, Result::ComputeSize(max_count));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  GLsizei count = 0;
  glGetAttachedShaders(
      program->service_id(), max_count, &count, result->GetData());
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
