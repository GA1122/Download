bool ParseDolbyVisionCodecId(const std::string& codec_id,
                             VideoCodecProfile* profile,
                             uint8_t* level_idc) {
  if (!IsDolbyVisionAVCCodecId(codec_id) &&
      !IsDolbyVisionHEVCCodecId(codec_id)) {
    return false;
  }

  const int kMaxDvCodecIdLength = 5      
                                  + 1    
                                  + 2    
                                  + 1    
                                  + 2;   

  if (codec_id.size() > kMaxDvCodecIdLength) {
    DVLOG(4) << __func__ << ": Codec id is too long (" << codec_id << ")";
    return false;
  }

  std::vector<std::string> elem = base::SplitString(
      codec_id, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  DCHECK(elem[0] == "dvh1" || elem[0] == "dvhe" || elem[0] == "dva1" ||
         elem[0] == "dvav");

  if (elem.size() != 3) {
    DVLOG(4) << __func__ << ": invalid dolby vision codec id " << codec_id;
    return false;
  }

  unsigned profile_id = 0;
  if (elem[1].size() != 2 || !base::StringToUint(elem[1], &profile_id) ||
      profile_id > 7) {
    DVLOG(4) << __func__ << ": invalid format or profile_id=" << elem[1];
    return false;
  }

  switch (profile_id) {
    case 0:
      if (!IsDolbyVisionAVCCodecId(codec_id)) {
        DVLOG(4) << __func__
                 << ": codec id is mismatched with profile_id=" << profile_id;
        return false;
      }
      *profile = DOLBYVISION_PROFILE0;
      break;
#if BUILDFLAG(ENABLE_HEVC_DEMUXING)
    case 4:
    case 5:
    case 7:
      if (!IsDolbyVisionHEVCCodecId(codec_id)) {
        DVLOG(4) << __func__
                 << ": codec id is mismatched with profile_id=" << profile_id;
        return false;
      }
      if (profile_id == 4)
        *profile = DOLBYVISION_PROFILE4;
      else if (profile_id == 5)
        *profile = DOLBYVISION_PROFILE5;
      else if (profile_id == 7)
        *profile = DOLBYVISION_PROFILE7;
      break;
#endif
    default:
      DVLOG(4) << __func__
               << ": depecrated and not supported profile_id=" << profile_id;
      return false;
  }

  unsigned level_id = 0;
  if (elem[2].size() != 2 || !base::StringToUint(elem[2], &level_id) ||
      level_id > 9 || level_id < 1) {
    DVLOG(4) << __func__ << ": invalid format level_id=" << elem[2];
    return false;
  }

  *level_idc = level_id;

  return true;
}
