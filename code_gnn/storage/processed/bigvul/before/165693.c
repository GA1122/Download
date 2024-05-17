std::vector<std::string> TokenizeString(const std::string& str,
                                        char delimiter,
                                        bool trim_spaces) {
  return TokenizeStringT<std::string>(str, delimiter, trim_spaces);
}
