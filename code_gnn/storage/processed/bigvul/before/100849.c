std::string HttpUtil::StripHeaders(const std::string& headers,
                                   const char* const headers_to_remove[],
                                   size_t headers_to_remove_len) {
  std::string stripped_headers;
  net::HttpUtil::HeadersIterator it(headers.begin(), headers.end(), "\r\n");

  while (it.GetNext()) {
    bool should_remove = false;
    for (size_t i = 0; i < headers_to_remove_len; ++i) {
      if (LowerCaseEqualsASCII(it.name_begin(), it.name_end(),
                               headers_to_remove[i])) {
        should_remove = true;
        break;
      }
    }
    if (!should_remove) {
      stripped_headers.append(it.name_begin(), it.values_end());
      stripped_headers.append("\r\n");
    }
  }
  return stripped_headers;
}
