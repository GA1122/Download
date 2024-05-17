error::Error GLES2DecoderImpl::HandleGetRequestableExtensionsCHROMIUM(
    uint32 immediate_data_size,
    const cmds::GetRequestableExtensionsCHROMIUM& c) {
  Bucket* bucket = CreateBucket(c.bucket_id);
  scoped_refptr<FeatureInfo> info(new FeatureInfo());
  info->Initialize(disallowed_features_);
  bucket->SetFromString(info->extensions().c_str());
  return error::kNoError;
}
