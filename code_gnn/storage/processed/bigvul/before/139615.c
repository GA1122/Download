bool ParseHEVCCodecId(const std::string& codec_id,
                      VideoCodecProfile* profile,
                      uint8_t* level_idc) {
  if (!base::StartsWith(codec_id, "hev1.", base::CompareCase::SENSITIVE) &&
      !base::StartsWith(codec_id, "hvc1.", base::CompareCase::SENSITIVE)) {
    return false;
  }

  const int kMaxHevcCodecIdLength =
      5 +   
      4 +   
      9 +   
      5 +   
      18;   

  if (codec_id.size() > kMaxHevcCodecIdLength) {
    DVLOG(4) << __func__ << ": Codec id is too long (" << codec_id << ")";
    return false;
  }

  std::vector<std::string> elem = base::SplitString(
      codec_id, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  DCHECK(elem[0] == "hev1" || elem[0] == "hvc1");

  if (elem.size() < 4) {
    DVLOG(4) << __func__ << ": invalid HEVC codec id " << codec_id;
    return false;
  }

  uint8_t general_profile_space = 0;
  if (elem[1].size() > 0 &&
      (elem[1][0] == 'A' || elem[1][0] == 'B' || elem[1][0] == 'C')) {
    general_profile_space = 1 + (elem[1][0] - 'A');
    elem[1].erase(0, 1);
  }
  DCHECK(general_profile_space >= 0 && general_profile_space <= 3);

  unsigned general_profile_idc = 0;
  if (!base::StringToUint(elem[1], &general_profile_idc) ||
      general_profile_idc > 0x1f) {
    DVLOG(4) << __func__ << ": invalid general_profile_idc=" << elem[1];
    return false;
  }

  uint32_t general_profile_compatibility_flags = 0;
  if (!base::HexStringToUInt(elem[2], &general_profile_compatibility_flags)) {
    DVLOG(4) << __func__
             << ": invalid general_profile_compatibility_flags=" << elem[2];
    return false;
  }

  if (profile) {
    if (general_profile_idc == 3 || (general_profile_compatibility_flags & 4)) {
      *profile = HEVCPROFILE_MAIN_STILL_PICTURE;
    }
    if (general_profile_idc == 2 || (general_profile_compatibility_flags & 2)) {
      *profile = HEVCPROFILE_MAIN10;
    }
    if (general_profile_idc == 1 || (general_profile_compatibility_flags & 1)) {
      *profile = HEVCPROFILE_MAIN;
    }
  }

  uint8_t general_tier_flag;
  if (elem[3].size() > 0 && (elem[3][0] == 'L' || elem[3][0] == 'H')) {
    general_tier_flag = (elem[3][0] == 'L') ? 0 : 1;
    elem[3].erase(0, 1);
  } else {
    DVLOG(4) << __func__ << ": invalid general_tier_flag=" << elem[3];
    return false;
  }
  DCHECK(general_tier_flag == 0 || general_tier_flag == 1);

  unsigned general_level_idc = 0;
  if (!base::StringToUint(elem[3], &general_level_idc) ||
      general_level_idc > 0xff) {
    DVLOG(4) << __func__ << ": invalid general_level_idc=" << elem[3];
    return false;
  }

  if (level_idc)
    *level_idc = static_cast<uint8_t>(general_level_idc);

  uint8_t constraint_flags[6];
  memset(constraint_flags, 0, sizeof(constraint_flags));

  if (elem.size() > 10) {
    DVLOG(4) << __func__ << ": unexpected number of trailing bytes in HEVC "
             << "codec id " << codec_id;
    return false;
  }
  for (size_t i = 4; i < elem.size(); ++i) {
    unsigned constr_byte = 0;
    if (!base::HexStringToUInt(elem[i], &constr_byte) || constr_byte > 0xFF) {
      DVLOG(4) << __func__ << ": invalid constraint byte=" << elem[i];
      return false;
    }
    constraint_flags[i - 4] = constr_byte;
  }

  return true;
}
