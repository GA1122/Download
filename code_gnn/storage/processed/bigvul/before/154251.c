error::Error GLES2DecoderImpl::HandleGetUniformsES3CHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::GetUniformsES3CHROMIUM& c =
      *static_cast<const volatile gles2::cmds::GetUniformsES3CHROMIUM*>(
          cmd_data);
  GLuint program_id = static_cast<GLuint>(c.program);
  uint32_t bucket_id = c.bucket_id;
  Bucket* bucket = CreateBucket(bucket_id);
  bucket->SetSize(sizeof(UniformsES3Header));   
  Program* program = nullptr;
  program = GetProgram(program_id);
  if (!program || !program->IsValid()) {
    return error::kNoError;
  }
  program->GetUniformsES3(bucket);
  return error::kNoError;
}
