static bool SniffForMagicNumbers(const char* content,
                                 size_t size,
                                 bool* have_enough_content,
                                 std::string* result) {
  *have_enough_content &= TruncateSize(kBytesRequiredForMagic, &size);

  return CheckForMagicNumbers(content, size, kMagicNumbers,
                              arraysize(kMagicNumbers), result);
}
