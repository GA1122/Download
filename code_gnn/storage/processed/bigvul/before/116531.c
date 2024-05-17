std::string ChromeContentClient::GetUserAgent(bool* overriding) const {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kUserAgent)) {
    *overriding = true;
    return CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
        switches::kUserAgent);
  } else {
    *overriding = false;
    chrome::VersionInfo version_info;
    std::string product("Chrome/");
    product += version_info.is_valid() ? version_info.Version() : "0.0.0.0";
    return webkit_glue::BuildUserAgentFromProduct(product);
  }
}
