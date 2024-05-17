VaapiWrapper::GetSupportedEncodeProfiles() {
  media::VideoEncodeAccelerator::SupportedProfiles profiles;
  const base::CommandLine* cmd_line = base::CommandLine::ForCurrentProcess();
  if (cmd_line->HasSwitch(switches::kDisableVaapiAcceleratedVideoEncode))
    return profiles;

  std::vector<ProfileInfo> encode_profile_infos =
      profile_infos_.Get().GetSupportedProfileInfosForCodecMode(kEncode);

  for (size_t i = 0; i < arraysize(kProfileMap); ++i) {
    VAProfile va_profile = ProfileToVAProfile(kProfileMap[i].profile, kEncode);
    if (va_profile == VAProfileNone)
      continue;
    for (const auto& profile_info : encode_profile_infos) {
      if (profile_info.va_profile == va_profile) {
        media::VideoEncodeAccelerator::SupportedProfile profile;
        profile.profile = kProfileMap[i].profile;
        profile.max_resolution = profile_info.max_resolution;
        profile.max_framerate_numerator = kMaxEncoderFramerate;
        profile.max_framerate_denominator = 1;
        profiles.push_back(profile);
        break;
      }
    }
  }
  return profiles;
}
