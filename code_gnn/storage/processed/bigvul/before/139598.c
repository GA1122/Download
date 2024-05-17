bool MimeUtil::IsCodecSupportedOnAndroid(
    Codec codec,
    const std::string& mime_type_lower_case,
    bool is_encrypted,
    const PlatformInfo& platform_info) {
  DCHECK_NE(mime_type_lower_case, "");

  if (is_encrypted && !platform_info.has_platform_decoders)
    return false;


  switch (codec) {
    case INVALID_CODEC:
    case AC3:
    case EAC3:
    case THEORA:
      return false;


    case PCM:
    case MP3:
    case MPEG4_AAC:
    case FLAC:
    case VORBIS:
      DCHECK(!is_encrypted || platform_info.has_platform_decoders);
      return true;

    case MPEG2_AAC:
      if (base::EndsWith(mime_type_lower_case, "mpegurl",
                         base::CompareCase::SENSITIVE)) {
        return false;
      }
      return !is_encrypted;

    case OPUS:
      if (!is_encrypted)
        return true;

      if (!platform_info.supports_opus)
        return false;

      if (base::EndsWith(mime_type_lower_case, "ogg",
                         base::CompareCase::SENSITIVE)) {
        return false;
      }

      DCHECK(!is_encrypted || platform_info.has_platform_decoders);
      return true;

    case H264:
      return !is_encrypted || platform_info.has_platform_decoders;

    case HEVC:
#if BUILDFLAG(ENABLE_HEVC_DEMUXING)
      if (!platform_info.has_platform_decoders)
        return false;

#if defined(OS_ANDROID)
      return base::android::BuildInfo::GetInstance()->sdk_int() >= 21;
#else
      return true;
#endif   
#else
      return false;
#endif   

    case VP8:
      if (!is_encrypted)
        return true;

      if (is_encrypted)
        return platform_info.has_platform_vp8_decoder;

      return true;

    case VP9: {
      if (base::CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kReportVp9AsAnUnsupportedMimeType)) {
        return false;
      }

      if (!is_encrypted)
        return true;

      if (!platform_info.has_platform_vp9_decoder)
        return false;

      if (is_encrypted)
        return true;

      return mime_type_lower_case == "video/webm";
    }

    case DOLBY_VISION:
      return false;
  }

  return false;
}
