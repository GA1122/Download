error::Error GLES2DecoderImpl::HandleBindFragmentInputLocationCHROMIUMBucket(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::BindFragmentInputLocationCHROMIUMBucket& c =
      *static_cast<
          const volatile gles2::cmds::BindFragmentInputLocationCHROMIUMBucket*>(
          cmd_data);
  if (!features().chromium_path_rendering) {
    return error::kUnknownCommand;
  }

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
  DoBindFragmentInputLocationCHROMIUM(program, location, name_str);
  return error::kNoError;
}
