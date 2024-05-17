error::Error GLES2DecoderImpl::HandleGetRequestableExtensionsCHROMIUM(
    uint32 immediate_data_size,
    const gles2::GetRequestableExtensionsCHROMIUM& c) {
  Bucket* bucket = CreateBucket(c.bucket_id);
  FeatureInfo::Ref info(new FeatureInfo());
  info->Initialize(disallowed_features_, NULL);
  bucket->SetFromString(info->extensions().c_str());
  return error::kNoError;
}
