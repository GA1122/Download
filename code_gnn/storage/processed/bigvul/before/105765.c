void SplitStringDontTrim(const std::string& str,
                         char c,
                         std::vector<std::string>* r) {
  DCHECK(IsStringUTF8(str));
  DCHECK(c >= 0 && c < 0x7F);
  SplitStringT(str, c, false, r);
}
