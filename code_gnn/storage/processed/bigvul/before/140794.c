error::Error GLES2DecoderImpl::HandleGetActiveUniformBlockName(
    uint32 immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetActiveUniformBlockName& c =
      *static_cast<const gles2::cmds::GetActiveUniformBlockName*>(cmd_data);
  GLuint program_id = c.program;
  GLuint index = c.index;
  uint32 name_bucket_id = c.name_bucket_id;
  typedef cmds::GetActiveUniformBlockName::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (*result != 0) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetActiveUniformBlockName");
  if (!program) {
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  GLint link_status = GL_FALSE;
  glGetProgramiv(service_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
        "glGetActiveActiveUniformBlockName", "program not linked");
    return error::kNoError;
  }
  GLint max_length = 0;
  glGetProgramiv(
      service_id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_length);
  GLsizei buf_size = static_cast<GLsizei>(max_length) + 1;
  std::vector<char> buffer(buf_size);
  GLsizei length = 0;
  glGetActiveUniformBlockName(
      service_id, index, buf_size, &length, &buffer[0]);
  if (length == 0) {
    *result = 0;
    return error::kNoError;
  }
  *result = 1;
  Bucket* bucket = CreateBucket(name_bucket_id);
  DCHECK_GT(buf_size, length);
  DCHECK_EQ(0, buffer[length]);
  bucket->SetFromString(&buffer[0]);
  return error::kNoError;
}
