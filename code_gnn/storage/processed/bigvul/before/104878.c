string16 ToLower(const string16& string) {
  icu::UnicodeString unicode_string(string.c_str(), string.size());
  unicode_string.toLower();
  return string16(unicode_string.getBuffer(), unicode_string.length());
}
