static bool SniffXML(const char* content,
                     size_t size,
                     bool* have_enough_content,
                     std::string* result) {
  *have_enough_content &= TruncateSize(300, &size);
  const char* pos = content;
  const char* const end = content + size;

  const int kMaxTagIterations = 5;
  for (int i = 0; i < kMaxTagIterations && pos < end; ++i) {
    pos = reinterpret_cast<const char*>(memchr(pos, '<', end - pos));
    if (!pos)
      return false;

    static const char kXmlPrefix[] = "<?xml";
    static const size_t kXmlPrefixLength = arraysize(kXmlPrefix) - 1;
    static const char kDocTypePrefix[] = "<!DOCTYPE";
    static const size_t kDocTypePrefixLength = arraysize(kDocTypePrefix) - 1;

    if ((pos + kXmlPrefixLength <= end) &&
        base::EqualsCaseInsensitiveASCII(
            base::StringPiece(pos, kXmlPrefixLength),
            base::StringPiece(kXmlPrefix, kXmlPrefixLength))) {
      ++pos;
      continue;
    } else if ((pos + kDocTypePrefixLength <= end) &&
               base::EqualsCaseInsensitiveASCII(
                   base::StringPiece(pos, kDocTypePrefixLength),
                   base::StringPiece(kDocTypePrefix, kDocTypePrefixLength))) {
      ++pos;
      continue;
    }

    if (CheckForMagicNumbers(pos, end - pos, kMagicXML, arraysize(kMagicXML),
                             result))
      return true;


    return true;
  }

  return pos < end;
}
