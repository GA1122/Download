static bool MagicMaskCmp(const char* magic_entry,
                         const char* content,
                         size_t len,
                         const char* mask) {
  while (len) {
    if ((*magic_entry != '.') && (*magic_entry != (*mask & *content)))
      return false;
    ++magic_entry;
    ++content;
    ++mask;
    --len;
  }
  return true;
}
