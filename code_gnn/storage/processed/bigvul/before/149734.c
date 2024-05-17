void FeatureInfo::Initialize(ContextType context_type,
                             bool is_passthrough_cmd_decoder,
                             const DisallowedFeatures& disallowed_features,
                             bool force_reinitialize) {
  if (initialized_) {
    DCHECK_EQ(context_type, context_type_);
    DCHECK_EQ(is_passthrough_cmd_decoder, is_passthrough_cmd_decoder_);
    DCHECK(disallowed_features == disallowed_features_);
    if (!force_reinitialize)
      return;
  }

  disallowed_features_ = disallowed_features;
  context_type_ = context_type;
  is_passthrough_cmd_decoder_ = is_passthrough_cmd_decoder;
  InitializeFeatures();
  initialized_ = true;
}
