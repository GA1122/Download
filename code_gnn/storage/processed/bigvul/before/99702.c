VaapiWrapper::GetSupportedProfileInfosForCodecModeInternal(CodecMode mode) {
  std::vector<ProfileInfo> supported_profile_infos;
  std::vector<VAProfile> va_profiles;
  if (!GetSupportedVaProfiles(&va_profiles))
    return supported_profile_infos;

  std::vector<VAConfigAttrib> required_attribs = GetRequiredAttribs(mode);
  VAEntrypoint entrypoint =
      (mode == kEncode ? VAEntrypointEncSlice: VAEntrypointVLD);

  base::AutoLock auto_lock(*va_lock_);
  for (const auto& va_profile : va_profiles) {
    if (!IsEntrypointSupported_Locked(va_profile, entrypoint))
      continue;
    if (!AreAttribsSupported_Locked(va_profile, entrypoint, required_attribs))
      continue;
    ProfileInfo profile_info;
    if (!GetMaxResolution_Locked(va_profile,
                                 entrypoint,
                                 required_attribs,
                                 &profile_info.max_resolution)) {
      LOG(ERROR) << "GetMaxResolution failed for va_profile " << va_profile
                 << " and entrypoint " << entrypoint;
      continue;
    }
    profile_info.va_profile = va_profile;
    supported_profile_infos.push_back(profile_info);
  }
  return supported_profile_infos;
}
