error::Error GLES2DecoderImpl::HandleGetFragDataIndexEXT(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!features().ext_blend_func_extended) {
    return error::kUnknownCommand;
  }
  const volatile gles2::cmds::GetFragDataIndexEXT& c =
      *static_cast<const volatile gles2::cmds::GetFragDataIndexEXT*>(cmd_data);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  return GetFragDataIndexHelper(c.program, c.index_shm_id, c.index_shm_offset,
                                name_str);
}
