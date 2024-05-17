const std::vector<std::string> CSPHandler::Keys() const {
  const std::string& key = is_platform_app_ ?
      keys::kPlatformAppContentSecurityPolicy : keys::kContentSecurityPolicy;
  return SingleKey(key);
}
