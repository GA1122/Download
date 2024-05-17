error::Error GLES2DecoderImpl::HandleGetActiveAttrib(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetActiveAttrib& c =
      *static_cast<const volatile gles2::cmds::GetActiveAttrib*>(cmd_data);
  GLuint program_id = c.program;
  GLuint index = c.index;
  uint32_t name_bucket_id = c.name_bucket_id;
  typedef cmds::GetActiveAttrib::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->success != 0) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetActiveAttrib");
  if (!program) {
    return error::kNoError;
  }
  const Program::VertexAttrib* attrib_info =
      program->GetAttribInfo(index);
  if (!attrib_info) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetActiveAttrib", "index out of range");
    return error::kNoError;
  }
  result->success = 1;   
  result->size = attrib_info->size;
  result->type = attrib_info->type;
  Bucket* bucket = CreateBucket(name_bucket_id);
  bucket->SetFromString(attrib_info->name.c_str());
  return error::kNoError;
}
