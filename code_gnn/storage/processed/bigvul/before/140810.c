error::Error GLES2DecoderImpl::HandleGetUniformIndices(
    uint32 immediate_data_size,
    const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetUniformIndices& c =
      *static_cast<const gles2::cmds::GetUniformIndices*>(cmd_data);
  Bucket* bucket = GetBucket(c.names_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  GLsizei count = 0;
  std::vector<char*> names;
  std::vector<GLint> len;
  if (!bucket->GetAsStrings(&count, &names, &len) || count <= 0) {
    return error::kInvalidArguments;
  }
  typedef cmds::GetUniformIndices::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.indices_shm_id, c.indices_shm_offset,
      Result::ComputeSize(static_cast<size_t>(count)));
  GLuint* indices = result ? result->GetData() : NULL;
  if (indices == NULL) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(c.program, "glGetUniformIndices");
  if (!program) {
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  GLint link_status = GL_FALSE;
  glGetProgramiv(service_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
        "glGetUniformIndices", "program not linked");
    return error::kNoError;
  }
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("GetUniformIndices");
  glGetUniformIndices(service_id, count, &names[0], indices);
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) {
    result->SetNumResults(count);
  } else {
    LOCAL_SET_GL_ERROR(error, "GetUniformIndices", "");
  }
  return error::kNoError;
}