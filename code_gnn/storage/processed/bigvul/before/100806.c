bool HttpResponseHeaders::HasStrongValidators() const {
  if (GetHttpVersion() < HttpVersion(1, 1))
    return false;

  std::string etag_value;
  EnumerateHeader(NULL, "etag", &etag_value);
  if (!etag_value.empty()) {
    size_t slash = etag_value.find('/');
    if (slash == std::string::npos || slash == 0)
      return true;

    std::string::const_iterator i = etag_value.begin();
    std::string::const_iterator j = etag_value.begin() + slash;
    HttpUtil::TrimLWS(&i, &j);
    if (!LowerCaseEqualsASCII(i, j, "w"))
      return true;
  }

  Time last_modified;
  if (!GetLastModifiedValue(&last_modified))
    return false;

  Time date;
  if (!GetDateValue(&date))
    return false;

  return ((date - last_modified).InSeconds() >= 60);
}
