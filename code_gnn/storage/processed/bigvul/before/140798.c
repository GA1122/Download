error::Error GLES2DecoderImpl::HandleGetFragDataLocation(
    uint32 immediate_data_size,
    const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::GetFragDataLocation& c =
      *static_cast<const gles2::cmds::GetFragDataLocation*>(cmd_data);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  return GetFragDataLocationHelper(
      c.program, c.location_shm_id, c.location_shm_offset, name_str);
}
