error::Error GLES2DecoderImpl::HandleGetProgramInfoLog(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::GetProgramInfoLog& c =
      *static_cast<const gles2::cmds::GetProgramInfoLog*>(cmd_data);
  GLuint program_id = c.program;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetProgramInfoLog");
  if (!program || !program->log_info()) {
    bucket->SetFromString("");
    return error::kNoError;
  }
  bucket->SetFromString(program->log_info()->c_str());
  return error::kNoError;
}
