void WebMediaPlayerImpl::RecordEncryptionScheme(
    const std::string& stream_name,
    const EncryptionScheme& encryption_scheme) {
  DCHECK(stream_name == "Audio" || stream_name == "Video");

  if (encryption_scheme.mode() == EncryptionScheme::CIPHER_MODE_UNENCRYPTED)
    return;

  base::UmaHistogramEnumeration(
      "Media.EME.EncryptionScheme.Initial." + stream_name,
      DetermineEncryptionSchemeUMAValue(encryption_scheme),
      EncryptionSchemeUMA::kCount);
}
