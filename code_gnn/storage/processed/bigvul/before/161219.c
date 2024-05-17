bool SniffMimeTypeFromLocalData(const char* content,
                                size_t size,
                                std::string* result) {
  if (CheckForMagicNumbers(content, size, kExtraMagicNumbers,
                           arraysize(kExtraMagicNumbers), result))
    return true;
  return CheckForMagicNumbers(content, size, kMagicNumbers,
                              arraysize(kMagicNumbers), result);
}
