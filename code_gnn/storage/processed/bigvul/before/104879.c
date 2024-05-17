string16 ToUpper(const string16& string) {
  icu::UnicodeString unicode_string(string.c_str(), string.size());
  unicode_string.toUpper();
  return string16(unicode_string.getBuffer(), unicode_string.length());
}
