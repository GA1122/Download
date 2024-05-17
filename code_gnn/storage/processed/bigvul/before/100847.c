bool HttpUtil::ParseRanges(const std::string& headers,
                           std::vector<HttpByteRange>* ranges) {
  std::string ranges_specifier;
  HttpUtil::HeadersIterator it(headers.begin(), headers.end(), "\r\n");

  while (it.GetNext()) {
    if (!LowerCaseEqualsASCII(it.name(), "range"))
      continue;
    ranges_specifier = it.values();
    break;
  }

  if (ranges_specifier.empty())
    return false;

  return ParseRangeHeader(ranges_specifier, ranges);
}
