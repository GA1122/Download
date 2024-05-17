error::Error GLES2DecoderImpl::HandleEnableFeatureCHROMIUM(
    uint32 immediate_data_size, const gles2::EnableFeatureCHROMIUM& c) {
  Bucket* bucket = GetBucket(c.bucket_id);
  typedef gles2::EnableFeatureCHROMIUM::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (*result != 0) {
    return error::kInvalidArguments;
  }
  std::string feature_str;
  if (!bucket->GetAsString(&feature_str)) {
    return error::kInvalidArguments;
  }

  if (feature_str.compare("pepper3d_allow_buffers_on_multiple_targets") == 0) {
    buffer_manager()->set_allow_buffers_on_multiple_targets(true);
  } else if (feature_str.compare("pepper3d_support_fixed_attribs") == 0) {
    buffer_manager()->set_allow_buffers_on_multiple_targets(true);
    const_cast<Validators*>(validators_)->vertex_attrib_type.AddValue(GL_FIXED);
  } else {
    return error::kNoError;
  }

  *result = 1;   
  return error::kNoError;
}