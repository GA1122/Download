bool MimeUtil::IsCodecProprietary(Codec codec) const {
  switch (codec) {
    case INVALID_CODEC:
    case AC3:
    case EAC3:
    case MP3:
    case MPEG2_AAC:
    case MPEG4_AAC:
    case H264:
    case HEVC:
    case DOLBY_VISION:
      return true;

    case PCM:
    case VORBIS:
    case OPUS:
    case FLAC:
    case VP8:
    case VP9:
    case THEORA:
      return false;
  }

  return true;
}
