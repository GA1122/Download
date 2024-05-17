std::string CryptohomeClient::GetStubSanitizedUsername(
    const std::string& username) {
  return username + kUserIdStubHashSuffix;
}
