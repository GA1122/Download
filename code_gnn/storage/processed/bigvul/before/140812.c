error::Error GLES2DecoderImpl::HandleGetUniformsES3CHROMIUM(
    uint32 immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetUniformsES3CHROMIUM& c =
      *static_cast<const gles2::cmds::GetUniformsES3CHROMIUM*>(cmd_data);
  GLuint program_id = static_cast<GLuint>(c.program);
  uint32 bucket_id = c.bucket_id;
  Bucket* bucket = CreateBucket(bucket_id);
  bucket->SetSize(sizeof(UniformsES3Header));   
  Program* program = NULL;
  program = GetProgram(program_id);
  if (!program || !program->IsValid()) {
    return error::kNoError;
  }
  program->GetUniformsES3(bucket);
  return error::kNoError;
}
