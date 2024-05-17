VAProfile VaapiWrapper::ProfileToVAProfile(
    media::VideoCodecProfile profile, CodecMode mode) {
  VAProfile va_profile = VAProfileNone;
  for (size_t i = 0; i < arraysize(kProfileMap); ++i) {
    if (kProfileMap[i].profile == profile) {
      va_profile = kProfileMap[i].va_profile;
      break;
    }
  }
  if (!profile_infos_.Get().IsProfileSupported(mode, va_profile) &&
      va_profile == VAProfileH264Baseline) {
    if (profile_infos_.Get().IsProfileSupported(
          mode, VAProfileH264ConstrainedBaseline)) {
      va_profile = VAProfileH264ConstrainedBaseline;
      DVLOG(1) << "Fall back to constrained baseline profile.";
    }
  }
  return va_profile;
}
