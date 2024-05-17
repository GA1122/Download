bool CanBeInManagedDomain(const std::string& username) {
  if (username.empty()) {
    return false;
  }
  for (size_t i = 0; i < arraysize(kNonManagedDomains); i++) {
    if (EndsWith(username, kNonManagedDomains[i], true)) {
      return false;
    }
  }
  return true;
}
