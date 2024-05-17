std::string HttpUtil::Quote(const std::string& str) {
  std::string escaped;
  escaped.reserve(2 + str.size());

  std::string::const_iterator begin = str.begin();
  std::string::const_iterator end = str.end();

  escaped.push_back('"');
  for (; begin != end; ++begin) {
    char c = *begin;
    if (c == '"' || c == '\\')
      escaped.push_back('\\');
    escaped.push_back(c);
  }
  escaped.push_back('"');
  return escaped;
}
