size_t GetUTF8Offset(const string16& text, size_t text_offset) {
  return UTF16ToUTF8(text.substr(0, text_offset)).length();
}
