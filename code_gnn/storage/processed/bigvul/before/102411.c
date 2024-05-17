std::string GetSpecificHeader(const std::string& headers,
                              const std::string& name) {
  if (headers.empty())
    return std::string();

  std::string match('\n' + name + ':');

  std::string::const_iterator begin =
      std::search(headers.begin(), headers.end(), match.begin(), match.end(),
             base::CaseInsensitiveCompareASCII<char>());

  if (begin == headers.end())
    return std::string();

  begin += match.length();

  std::string ret;
  TrimWhitespace(std::string(begin, std::find(begin, headers.end(), '\n')),
                 TRIM_ALL, &ret);
  return ret;
}
