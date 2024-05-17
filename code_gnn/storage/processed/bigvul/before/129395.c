error::Error GLES2DecoderImpl::HandleBindUniformLocationCHROMIUMBucket(
    uint32 immediate_data_size,
    const cmds::BindUniformLocationCHROMIUMBucket& c) {
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
