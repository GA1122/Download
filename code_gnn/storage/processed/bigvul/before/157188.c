EncryptionMode DetermineEncryptionMode(
    const EncryptionScheme& encryption_scheme) {
  switch (encryption_scheme.mode()) {
    case EncryptionScheme::CIPHER_MODE_UNENCRYPTED:
      return EncryptionMode::kUnencrypted;
    case EncryptionScheme::CIPHER_MODE_AES_CTR:
      return EncryptionMode::kCenc;
    case EncryptionScheme::CIPHER_MODE_AES_CBC:
      return EncryptionMode::kCbcs;
  }
}
