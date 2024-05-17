error::Error GLES2DecoderImpl::HandleGetFragDataLocation(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::GetFragDataLocation& c =
      *static_cast<const volatile gles2::cmds::GetFragDataLocation*>(cmd_data);
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
