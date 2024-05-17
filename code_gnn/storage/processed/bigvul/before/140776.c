error::Error GLES2DecoderImpl::HandleBindAttribLocationBucket(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::BindAttribLocationBucket& c =
      *static_cast<const gles2::cmds::BindAttribLocationBucket*>(cmd_data);
  GLuint program = static_cast<GLuint>(c.program);
  GLuint index = static_cast<GLuint>(c.index);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  DoBindAttribLocation(program, index, name_str.c_str());
  return error::kNoError;
}