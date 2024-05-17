std::string ChromeContentClient::GetUserAgent() const {
  chrome::VersionInfo version_info;
  std::string product("Chrome/");
  product += version_info.is_valid() ? version_info.Version() : "0.0.0.0";
  return webkit_glue::BuildUserAgentFromProduct(product);
}
