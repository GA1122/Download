SupportsType MimeUtil::IsSupportedMediaFormat(
    const std::string& mime_type,
    const std::vector<std::string>& codecs,
    bool is_encrypted) const {
  const std::string mime_type_lower_case = base::ToLowerASCII(mime_type);
  MediaFormatMappings::const_iterator it_media_format_map =
      media_format_map_.find(mime_type_lower_case);
  if (it_media_format_map == media_format_map_.end())
    return IsNotSupported;

  if (it_media_format_map->second.empty()) {
    if (codecs.empty()) {
      return IsDefaultCodecSupported(mime_type_lower_case, is_encrypted);
    } else {
      return IsNotSupported;
    }
  }

  if (codecs.empty()) {
    Codec default_codec = INVALID_CODEC;
    if (!GetDefaultCodec(mime_type_lower_case, &default_codec))
      return MayBeSupported;

    return IsSimpleCodecSupported(mime_type_lower_case, default_codec,
                                  is_encrypted);
  }

#if BUILDFLAG(ENABLE_MSE_MPEG2TS_STREAM_PARSER)
  if (mime_type_lower_case == "video/mp2t") {
    std::vector<std::string> codecs_to_check;
    for (const auto& codec_id : codecs) {
      codecs_to_check.push_back(TranslateLegacyAvc1CodecIds(codec_id));
    }
    return AreSupportedCodecs(it_media_format_map->second, codecs_to_check,
                              mime_type_lower_case, is_encrypted);
  }
#endif

  return AreSupportedCodecs(it_media_format_map->second, codecs,
                            mime_type_lower_case, is_encrypted);
}
