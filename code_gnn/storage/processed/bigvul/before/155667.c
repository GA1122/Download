static bool IsValidUTF16(const base::string16& str16) {
  std::string unused_str8;
  return base::UTF16ToUTF8(str16.c_str(), str16.size(), &unused_str8);
}
