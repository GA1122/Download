bool IsPhishingList(const std::string& list_name) {
  return list_name.find("-phish-") != std::string::npos;
}
