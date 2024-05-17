static bool CheckForMagicNumbers(const char* content, size_t size,
                                 const MagicNumber* magic, size_t magic_len,
                                 std::string* result) {
  for (size_t i = 0; i < magic_len; ++i) {
    if (MatchMagicNumber(content, size, magic[i], result))
      return true;
  }
  return false;
}
