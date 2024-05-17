bool VaapiWrapper::LazyProfileInfos::IsProfileSupported(
    CodecMode mode, VAProfile va_profile) {
  for (const auto& profile : supported_profiles_[mode]) {
    if (profile.va_profile == va_profile)
      return true;
  }
  return false;
}
