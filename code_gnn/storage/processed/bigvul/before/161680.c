unsigned int GetVaFormatForVideoCodecProfile(VideoCodecProfile profile) {
  if (profile == VP9PROFILE_PROFILE2 || profile == VP9PROFILE_PROFILE3)
    return VA_RT_FORMAT_YUV420_10BPP;
  return VA_RT_FORMAT_YUV420;
}
