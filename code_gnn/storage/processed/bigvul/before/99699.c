VaapiWrapper::GetSupportedDecodeProfiles() {
  media::VideoDecodeAccelerator::SupportedProfiles profiles;
  std::vector<ProfileInfo> decode_profile_infos =
      profile_infos_.Get().GetSupportedProfileInfosForCodecMode(kDecode);

  for (size_t i = 0; i < arraysize(kProfileMap); ++i) {
    VAProfile va_profile = ProfileToVAProfile(kProfileMap[i].profile, kDecode);
    if (va_profile == VAProfileNone)
      continue;
    for (const auto& profile_info : decode_profile_infos) {
      if (profile_info.va_profile == va_profile) {
        media::VideoDecodeAccelerator::SupportedProfile profile;
        profile.profile = kProfileMap[i].profile;
        profile.max_resolution = profile_info.max_resolution;
        profile.min_resolution.SetSize(16, 16);
        profiles.push_back(profile);
        break;
      }
    }
  }
  return profiles;
}
