const std::map<std::string, MimeUtil::Codec>& GetStringToCodecMap() {
  static const std::map<std::string, MimeUtil::Codec> kStringToCodecMap = {
    {"1", MimeUtil::PCM},
    {"mp3", MimeUtil::MP3},
    {"mp4a.66", MimeUtil::MPEG2_AAC},
    {"mp4a.67", MimeUtil::MPEG2_AAC},
    {"mp4a.68", MimeUtil::MPEG2_AAC},
    {"mp4a.69", MimeUtil::MP3},
    {"mp4a.6B", MimeUtil::MP3},
    {"mp4a.40.2", MimeUtil::MPEG4_AAC},
    {"mp4a.40.02", MimeUtil::MPEG4_AAC},
    {"mp4a.40.5", MimeUtil::MPEG4_AAC},
    {"mp4a.40.05", MimeUtil::MPEG4_AAC},
    {"mp4a.40.29", MimeUtil::MPEG4_AAC},
#if BUILDFLAG(ENABLE_AC3_EAC3_AUDIO_DEMUXING)
    {"ac-3", MimeUtil::AC3},
    {"mp4a.a5", MimeUtil::AC3},
    {"mp4a.A5", MimeUtil::AC3},
    {"ec-3", MimeUtil::EAC3},
    {"mp4a.a6", MimeUtil::EAC3},
    {"mp4a.A6", MimeUtil::EAC3},
#endif
    {"vorbis", MimeUtil::VORBIS},
    {"opus", MimeUtil::OPUS},
    {"flac", MimeUtil::FLAC},
    {"vp8", MimeUtil::VP8},
    {"vp8.0", MimeUtil::VP8},
    {"theora", MimeUtil::THEORA}
  };

  return kStringToCodecMap;
}
