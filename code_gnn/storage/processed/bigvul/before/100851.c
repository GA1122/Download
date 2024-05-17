std::string HttpUtil::Unquote(std::string::const_iterator begin,
                              std::string::const_iterator end) {
  if (begin == end)
    return std::string();

  if (!IsQuote(*begin))
    return std::string(begin, end);

  if (end - begin < 2 || *begin != *(end - 1))
    return std::string(begin, end);

  ++begin;
  --end;

  std::string unescaped;
  bool prev_escape = false;
  for (; begin != end; ++begin) {
    char c = *begin;
    if (c == '\\' && !prev_escape) {
      prev_escape = true;
      continue;
    }
    prev_escape = false;
    unescaped.push_back(c);
  }
  return unescaped;
}
