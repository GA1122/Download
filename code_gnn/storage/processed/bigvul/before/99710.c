VaapiWrapper::LazyProfileInfos::LazyProfileInfos() {
  static_assert(arraysize(supported_profiles_) == kCodecModeMax,
                "The array size of supported profile is incorrect.");
  scoped_ptr<VaapiWrapper> vaapi_wrapper(new VaapiWrapper());
  if (!vaapi_wrapper->VaInitialize(base::Bind(&base::DoNothing)))
    return;
  for (size_t i = 0; i < kCodecModeMax; ++i) {
    supported_profiles_[i] =
        vaapi_wrapper->GetSupportedProfileInfosForCodecModeInternal(
            static_cast<CodecMode>(i));
  }
}
