EncryptionSchemeUMA DetermineEncryptionSchemeUMAValue(
    const EncryptionScheme& encryption_scheme) {
  if (encryption_scheme.mode() == EncryptionScheme::CIPHER_MODE_AES_CBC)
    return EncryptionSchemeUMA::kCbcs;

  DCHECK_EQ(encryption_scheme.mode(), EncryptionScheme::CIPHER_MODE_AES_CTR);
  return EncryptionSchemeUMA::kCenc;
}
