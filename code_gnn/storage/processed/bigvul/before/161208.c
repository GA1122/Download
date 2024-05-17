bool LooksLikeBinary(const char* content, size_t size) {
  const uint32_t kBinaryBits =
      ~(1u << '\t' | 1u << '\n' | 1u << '\r' | 1u << '\f' | 1u << '\x1b');
  for (size_t i = 0; i < size; ++i) {
    uint8_t byte = static_cast<uint8_t>(content[i]);
    if (byte < 0x20 && (kBinaryBits & (1u << byte)))
      return true;
  }
  return false;
}
