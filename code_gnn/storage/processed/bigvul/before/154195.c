error::Error GLES2DecoderImpl::HandleBindFragDataLocationEXTBucket(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!features().ext_blend_func_extended) {
    return error::kUnknownCommand;
  }
  const volatile gles2::cmds::BindFragDataLocationEXTBucket& c =
      *static_cast<const volatile gles2::cmds::BindFragDataLocationEXTBucket*>(
          cmd_data);
  GLuint program = static_cast<GLuint>(c.program);
  GLuint colorNumber = static_cast<GLuint>(c.colorNumber);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  return DoBindFragDataLocation(program, colorNumber, name_str);
}
