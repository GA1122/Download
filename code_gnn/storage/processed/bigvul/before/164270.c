 std::unique_ptr<SymmetricKey> DeserializePaddingKey(
    const std::string& raw_key) {
  return SymmetricKey::Import(kPaddingKeyAlgorithm, raw_key);
 }
