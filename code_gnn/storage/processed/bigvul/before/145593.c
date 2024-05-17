size_t GetStringPayloadLength(const std::string& str, bool is_unicode) {
  if (!is_unicode)
    return str.length();

  return base::UTF8ToUTF16(str).length() * 2;
}
