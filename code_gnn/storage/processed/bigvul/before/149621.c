GURL CreateRedirectURL(const std::string& scheme,
                       const std::string& host,
                       std::uint16_t port) {
  return GURL(scheme + "://" + host + ":" + base::NumberToString(port));
}
