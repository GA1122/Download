size_t GetStringPayloadLength(const base::string16& str, bool is_unicode) {
  if (is_unicode)
    return str.length() * 2;

  return base::UTF16ToUTF8(str).length();
}
