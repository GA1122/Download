static bool SniffCRX(const char* content,
                     size_t size,
                     const GURL& url,
                     const std::string& type_hint,
                     bool* have_enough_content,
                     std::string* result) {
  static const struct MagicNumber kCRXMagicNumbers[] = {
    MAGIC_NUMBER("application/x-chrome-extension", "Cr24\x02\x00\x00\x00")
  };

  if (!base::EndsWith(url.path_piece(), ".crx", base::CompareCase::SENSITIVE))
    return false;

  *have_enough_content &= TruncateSize(kBytesRequiredForMagic, &size);
  return CheckForMagicNumbers(content, size, kCRXMagicNumbers,
                              arraysize(kCRXMagicNumbers), result);
}
