bool ParseLegacyVp9CodecID(const std::string& codec_id,
                           VideoCodecProfile* profile,
                           uint8_t* level_idc) {
  if (codec_id == "vp9" || codec_id == "vp9.0") {
    *profile = VP9PROFILE_PROFILE0;
    *level_idc = 0;
    return true;
  }
  return false;
}
