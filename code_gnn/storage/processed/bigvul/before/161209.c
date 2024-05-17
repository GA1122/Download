static bool MagicCmp(const char* magic_entry, const char* content, size_t len) {
  while (len) {
    if ((*magic_entry != '.') && (*magic_entry != *content))
      return false;
    ++magic_entry;
    ++content;
    --len;
  }
  return true;
}
