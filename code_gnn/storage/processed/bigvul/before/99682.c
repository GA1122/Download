scoped_ptr<VaapiWrapper> VaapiWrapper::CreateForVideoCodec(
    CodecMode mode,
    media::VideoCodecProfile profile,
    const base::Closure& report_error_to_uma_cb) {
  VAProfile va_profile = ProfileToVAProfile(profile, mode);
  scoped_ptr<VaapiWrapper> vaapi_wrapper =
      Create(mode, va_profile, report_error_to_uma_cb);
  return vaapi_wrapper.Pass();
}
