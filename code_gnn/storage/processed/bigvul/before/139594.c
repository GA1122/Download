bool MimeUtil::GetDefaultCodec(const std::string& mime_type,
                               Codec* default_codec) const {
  if (mime_type == "audio/mpeg" || mime_type == "audio/mp3" ||
      mime_type == "audio/x-mp3") {
    *default_codec = MimeUtil::MP3;
    return true;
  }

  if (mime_type == "audio/aac") {
    *default_codec = MimeUtil::MPEG4_AAC;
    return true;
  }

  if (mime_type == "audio/flac") {
    *default_codec = MimeUtil::FLAC;
    return true;
  }

  return false;
}
