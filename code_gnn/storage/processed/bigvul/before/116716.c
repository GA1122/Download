std::string ShellContentClient::GetUserAgent(bool* overriding) const {
  *overriding = false;
  return std::string("Chrome/15.16.17.18");
}
