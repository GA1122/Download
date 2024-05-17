void HttpResponseHeaders::Parse(const std::string& raw_input) {
  raw_headers_.reserve(raw_input.size());

  std::string::const_iterator line_begin = raw_input.begin();
  std::string::const_iterator line_end =
      std::find(line_begin, raw_input.end(), '\0');
  bool has_headers = (line_end != raw_input.end() &&
                      (line_end + 1) != raw_input.end() &&
                      *(line_end + 1) != '\0');
  ParseStatusLine(line_begin, line_end, has_headers);

  if (line_end == raw_input.end()) {
    raw_headers_.push_back('\0');
    return;
  }

  size_t status_line_len = raw_headers_.size();

  raw_headers_.append(line_end + 1, raw_input.end());

  line_end = raw_headers_.begin() + status_line_len - 1;

  HttpUtil::HeadersIterator headers(line_end + 1, raw_headers_.end(),
                                    std::string(1, '\0'));
  while (headers.GetNext()) {
    AddHeader(headers.name_begin(),
              headers.name_end(),
              headers.values_begin(),
              headers.values_end());
  }
}
