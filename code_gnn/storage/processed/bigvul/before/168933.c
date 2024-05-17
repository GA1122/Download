bool DevToolsAgentHost::IsSupportedProtocolVersion(const std::string& version) {
  return version == "1.0" || version == "1.1" || version == "1.2" ||
         version == "1.3";
}
