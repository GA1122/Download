std::string GetHeaderParamValue(const std::string& header,
                                const std::string& param_name,
                                QuoteRule::Type quote_rule) {
  std::string::const_iterator param_begin =
      std::search(header.begin(), header.end(), param_name.begin(),
                  param_name.end(), base::CaseInsensitiveCompareASCII<char>());

  if (param_begin == header.end())
    return std::string();
  param_begin += param_name.length();

  std::string whitespace(" \t");
  size_t equals_offset =
      header.find_first_not_of(whitespace, param_begin - header.begin());
  if (equals_offset == std::string::npos || header[equals_offset] != '=')
    return std::string();

  size_t param_value_offset =
      header.find_first_not_of(whitespace, equals_offset + 1);
  if (param_value_offset == std::string::npos)
    return std::string();

  param_begin = header.begin() + param_value_offset;
  if (param_begin == header.end())
    return std::string();

  std::string::const_iterator param_end;
  if (*param_begin == '"' && quote_rule == QuoteRule::REMOVE_OUTER_QUOTES) {
    ++param_begin;   
    param_end = std::find(param_begin, header.end(), '"');
  } else {
    param_end = std::find(param_begin + 1, header.end(), ';');
  }

  return std::string(param_begin, param_end);
}
