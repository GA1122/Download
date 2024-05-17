error::Error GLES2DecoderImpl::HandleBindUniformLocationCHROMIUMBucket(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::BindUniformLocationCHROMIUMBucket& c =
      *static_cast<const gles2::cmds::BindUniformLocationCHROMIUMBucket*>(
          cmd_data);
  GLuint program = static_cast<GLuint>(c.program);
  GLint location = static_cast<GLint>(c.location);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  DoBindUniformLocationCHROMIUM(program, location, name_str.c_str());
  return error::kNoError;
}
