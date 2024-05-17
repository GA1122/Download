bool HttpResponseHeaders::IsRedirect(std::string* location) const {
  if (!IsRedirectResponseCode(response_code_))
    return false;

  size_t i = std::string::npos;
  do {
    i = FindHeader(++i, "location");
    if (i == std::string::npos)
      return false;
  } while (parsed_[i].value_begin == parsed_[i].value_end);

  if (location) {
    *location = EscapeNonASCII(
        std::string(parsed_[i].value_begin, parsed_[i].value_end));
  }

  return true;
}
