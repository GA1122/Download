error::Error GLES2DecoderImpl::HandleGetAttribLocation(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::GetAttribLocation& c =
      *static_cast<const gles2::cmds::GetAttribLocation*>(cmd_data);
  Bucket* bucket = GetBucket(c.name_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  std::string name_str;
  if (!bucket->GetAsString(&name_str)) {
    return error::kInvalidArguments;
  }
  return GetAttribLocationHelper(
    c.program, c.location_shm_id, c.location_shm_offset, name_str);
}
