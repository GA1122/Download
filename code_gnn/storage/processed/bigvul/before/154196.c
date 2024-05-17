error::Error GLES2DecoderImpl::HandleBindFragDataLocationIndexedEXTBucket(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!features().ext_blend_func_extended) {
    return error::kUnknownCommand;
  }
  const volatile gles2::cmds::BindFragDataLocationIndexedEXTBucket& c =
      *static_cast<
          const volatile gles2::cmds::BindFragDataLocationIndexedEXTBucket*>(
          cmd_data);
  GLuint program = static_cast<GLuint>(c.program);
  GLuint colorNumber = static_cast<GLuint>(c.colorNumber);
  GLuint index = static_cast<GLuint>(c.index);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  return DoBindFragDataLocationIndexed(program, colorNumber, index, name_str);
}
