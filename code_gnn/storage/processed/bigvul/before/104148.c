error::Error GLES2DecoderImpl::HandleRequestExtensionCHROMIUM(
    uint32 immediate_data_size, const gles2::RequestExtensionCHROMIUM& c) {
  Bucket* bucket = GetBucket(c.bucket_id);
  std::string feature_str;
  if (!bucket->GetAsString(&feature_str)) {
    return error::kInvalidArguments;
  }

  bool std_derivatives_enabled =
      feature_info_->feature_flags().oes_standard_derivatives;
  bool webglsl_enabled =
      feature_info_->feature_flags().chromium_webglsl;

  feature_info_->AddFeatures(feature_str.c_str());

  if (std_derivatives_enabled !=
          feature_info_->feature_flags().oes_standard_derivatives ||
      webglsl_enabled !=
          feature_info_->feature_flags().chromium_webglsl) {
    InitializeShaderTranslator();
  }

  UpdateCapabilities();

  return error::kNoError;
}
