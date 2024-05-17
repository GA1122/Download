HttpVersion HttpResponseHeaders::ParseVersion(
    std::string::const_iterator line_begin,
    std::string::const_iterator line_end) {
  std::string::const_iterator p = line_begin;


  if ((line_end - p < 4) || !LowerCaseEqualsASCII(p, p + 4, "http")) {
    DVLOG(1) << "missing status line";
    return HttpVersion();
  }

  p += 4;

  if (p >= line_end || *p != '/') {
    DVLOG(1) << "missing version";
    return HttpVersion();
  }

  std::string::const_iterator dot = std::find(p, line_end, '.');
  if (dot == line_end) {
    DVLOG(1) << "malformed version";
    return HttpVersion();
  }

  ++p;   
  ++dot;   

  if (!(*p >= '0' && *p <= '9' && *dot >= '0' && *dot <= '9')) {
    DVLOG(1) << "malformed version number";
    return HttpVersion();
  }

  uint16 major = *p - '0';
  uint16 minor = *dot - '0';

  return HttpVersion(major, minor);
}
