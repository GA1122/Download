error::Error GLES2DecoderImpl::HandleGetUniformBlockIndex(
    uint32 immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetUniformBlockIndex& c =
      *static_cast<const gles2::cmds::GetUniformBlockIndex*>(cmd_data);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  GLuint* index = GetSharedMemoryAs<GLuint*>(
      c.index_shm_id, c.index_shm_offset, sizeof(GLuint));
  if (!index) {
    return error::kOutOfBounds;
  }
  if (*index != GL_INVALID_INDEX) {
    return error::kGenericError;
  }
  Program* program = GetProgramInfoNotShader(
      c.program, "glGetUniformBlockIndex");
  if (!program) {
    return error::kNoError;
  }
  *index = glGetUniformBlockIndex(program->service_id(), name_str.c_str());
  return error::kNoError;
}
