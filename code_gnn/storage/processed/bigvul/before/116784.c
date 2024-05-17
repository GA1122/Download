std::string TestContentClient::GetUserAgent(bool* overriding) const {
  *overriding = false;
  return std::string("TestContentClient");
}
