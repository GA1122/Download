bool IsPortAllowedForScheme(int port, const std::string& url_scheme) {
  if (!IsPortValid(port))
    return false;

  if (g_explicitly_allowed_ports.Get().count(port) > 0)
    return true;

  if (base::LowerCaseEqualsASCII(url_scheme, url::kFtpScheme)) {
    for (int allowed_ftp_port : kAllowedFtpPorts) {
      if (allowed_ftp_port == port)
        return true;
    }
  }

  for (int restricted_port : kRestrictedPorts) {
    if (restricted_port == port)
      return false;
  }

  return true;
}
