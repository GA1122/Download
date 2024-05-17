error::Error GLES2DecoderPassthroughImpl::DoRequestExtensionCHROMIUM(
    const char* extension) {
  api()->glRequestExtensionANGLEFn(extension);

  FlushErrors();

  context_->ReinitializeDynamicBindings();
  feature_info_->Initialize(
      feature_info_->context_type(), true  ,
      feature_info_->disallowed_features(), true  );

  return error::kNoError;
}
