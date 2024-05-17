std::string AutomationProvider::GetProtocolVersion() {
  chrome::VersionInfo version_info;
  return version_info.Version().c_str();
}
