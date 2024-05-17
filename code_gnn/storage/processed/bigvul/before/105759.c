 void SplitString(const std::string& str,
                  char c,
                 std::vector<std::string>* r) {
  DCHECK(c >= 0 && c < 0x7F);
  SplitStringT(str, c, true, r);
}
