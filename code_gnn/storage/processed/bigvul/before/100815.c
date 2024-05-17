void HttpResponseHeaders::ParseStatusLine(
    std::string::const_iterator line_begin,
    std::string::const_iterator line_end,
    bool has_headers) {
  parsed_http_version_ = ParseVersion(line_begin, line_end);

  if (parsed_http_version_ == HttpVersion(0, 9) && !has_headers) {
    http_version_ = HttpVersion(0, 9);
    raw_headers_ = "HTTP/0.9";
  } else if (parsed_http_version_ >= HttpVersion(1, 1)) {
    http_version_ = HttpVersion(1, 1);
    raw_headers_ = "HTTP/1.1";
  } else {
    http_version_ = HttpVersion(1, 0);
    raw_headers_ = "HTTP/1.0";
  }
  if (parsed_http_version_ != http_version_) {
    DVLOG(1) << "assuming HTTP/" << http_version_.major_value() << "."
             << http_version_.minor_value();
  }

  std::string::const_iterator p = std::find(line_begin, line_end, ' ');

  if (p == line_end) {
    DVLOG(1) << "missing response status; assuming 200 OK";
    raw_headers_.append(" 200 OK");
    raw_headers_.push_back('\0');
    response_code_ = 200;
    return;
  }

  while (*p == ' ')
    ++p;

  std::string::const_iterator code = p;
  while (*p >= '0' && *p <= '9')
    ++p;

  if (p == code) {
    DVLOG(1) << "missing response status number; assuming 200";
    raw_headers_.append(" 200 OK");
    response_code_ = 200;
    return;
  }
  raw_headers_.push_back(' ');
  raw_headers_.append(code, p);
  raw_headers_.push_back(' ');
  base::StringToInt(code, p, &response_code_);

  while (*p == ' ')
    ++p;

  while (line_end > p && line_end[-1] == ' ')
    --line_end;

  if (p == line_end) {
    DVLOG(1) << "missing response status text; assuming OK";
    raw_headers_.append("OK");
  } else {
    raw_headers_.append(p, line_end);
  }

  raw_headers_.push_back('\0');
}
