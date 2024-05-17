VideoCodec MimeUtilToVideoCodec(MimeUtil::Codec codec) {
  switch (codec) {
    case MimeUtil::H264:
      return kCodecH264;
    case MimeUtil::HEVC:
      return kCodecHEVC;
    case MimeUtil::VP8:
      return kCodecVP8;
    case MimeUtil::VP9:
      return kCodecVP9;
    case MimeUtil::THEORA:
      return kCodecTheora;
    case MimeUtil::DOLBY_VISION:
      return kCodecDolbyVision;
    default:
      break;
  }
  return kUnknownVideoCodec;
}
