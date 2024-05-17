SupportsType MimeUtil::IsDefaultCodecSupported(const std::string& mime_type,
                                               bool is_encrypted) const {
  Codec default_codec = Codec::INVALID_CODEC;
  if (!GetDefaultCodec(mime_type, &default_codec))
    return IsNotSupported;
  return IsSimpleCodecSupported(mime_type, default_codec, is_encrypted);
}
