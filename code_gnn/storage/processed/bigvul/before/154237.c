error::Error GLES2DecoderImpl::HandleGetRequestableExtensionsCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetRequestableExtensionsCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::GetRequestableExtensionsCHROMIUM*>(
          cmd_data);
  Bucket* bucket = CreateBucket(c.bucket_id);
  scoped_refptr<FeatureInfo> info(
      new FeatureInfo(workarounds(), group_->gpu_feature_info()));
  DisallowedFeatures disallowed_features = feature_info_->disallowed_features();
  disallowed_features.AllowExtensions();
  info->Initialize(feature_info_->context_type(),
                   false  , disallowed_features);
  bucket->SetFromString(gfx::MakeExtensionString(info->extensions()).c_str());
  return error::kNoError;
}
