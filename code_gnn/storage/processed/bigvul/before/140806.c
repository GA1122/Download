error::Error GLES2DecoderImpl::HandleGetTransformFeedbackVarying(
    uint32 immediate_data_size,
    const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetTransformFeedbackVarying& c =
      *static_cast<const gles2::cmds::GetTransformFeedbackVarying*>(cmd_data);
  GLuint program_id = c.program;
  GLuint index = c.index;
  uint32 name_bucket_id = c.name_bucket_id;
  typedef cmds::GetTransformFeedbackVarying::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->success != 0) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetTransformFeedbackVarying");
  if (!program) {
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  GLint link_status = GL_FALSE;
  glGetProgramiv(service_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
        "glGetTransformFeedbackVarying", "program not linked");
    return error::kNoError;
  }
  GLint max_length = 0;
  glGetProgramiv(
      service_id, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, &max_length);
  max_length = std::max(1, max_length);
  std::vector<char> buffer(max_length);
  GLsizei length = 0;
  GLsizei size = 0;
  GLenum type = 0;
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("GetTransformFeedbackVarying");
  glGetTransformFeedbackVarying(
      service_id, index, max_length, &length, &size, &type, &buffer[0]);
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    LOCAL_SET_GL_ERROR(error, "glGetTransformFeedbackVarying", "");
    return error::kNoError;
  }
  result->success = 1;   
  result->size = static_cast<int32_t>(size);
  result->type = static_cast<uint32_t>(type);
  Bucket* bucket = CreateBucket(name_bucket_id);
  DCHECK(length >= 0 && length < max_length);
  buffer[length] = '\0';   
  bucket->SetFromString(&buffer[0]);
  return error::kNoError;
}
