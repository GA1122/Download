scoped_ptr<VaapiWrapper> VaapiWrapper::Create(
    CodecMode mode,
    VAProfile va_profile,
    const base::Closure& report_error_to_uma_cb) {
  if (!profile_infos_.Get().IsProfileSupported(mode, va_profile)) {
    DVLOG(1) << "Unsupported va_profile: " << va_profile;
    return nullptr;
  }

  scoped_ptr<VaapiWrapper> vaapi_wrapper(new VaapiWrapper());
  if (vaapi_wrapper->VaInitialize(report_error_to_uma_cb)) {
    if (vaapi_wrapper->Initialize(mode, va_profile))
      return vaapi_wrapper.Pass();
  }
  LOG(ERROR) << "Failed to create VaapiWrapper for va_profile: " << va_profile;
  return nullptr;
}
