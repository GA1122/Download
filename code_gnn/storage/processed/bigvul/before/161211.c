static bool MatchMagicNumber(const char* content,
                             size_t size,
                             const MagicNumber& magic_entry,
                             std::string* result) {
  const size_t len = magic_entry.magic_len;

  DCHECK_LE(len, kBytesRequiredForMagic);

  const char* end = static_cast<const char*>(memchr(content, '\0', size));
  const size_t content_strlen =
      (end != NULL) ? static_cast<size_t>(end - content) : size;

  bool match = false;
  if (magic_entry.is_string) {
    if (content_strlen >= len) {
      DCHECK_EQ(strlen(magic_entry.magic), len);
      match = base::EqualsCaseInsensitiveASCII(magic_entry.magic,
                                               base::StringPiece(content, len));
    }
  } else {
    if (size >= len) {
      if (!magic_entry.mask) {
        match = MagicCmp(magic_entry.magic, content, len);
      } else {
        match = MagicMaskCmp(magic_entry.magic, content, len, magic_entry.mask);
      }
    }
  }

  if (match) {
    result->assign(magic_entry.mime_type);
    return true;
  }
  return false;
}
