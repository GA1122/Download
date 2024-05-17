void Automation::GetBrowserVersion(std::string* version) {
  *version = automation()->server_version();
}
