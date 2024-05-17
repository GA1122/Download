bool IsMalwareList(const std::string& list_name) {
  return list_name.find("-malware-") != std::string::npos;
}
