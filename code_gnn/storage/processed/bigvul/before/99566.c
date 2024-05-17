int GetListId(const std::string& name) {
  if (name == kMalwareList)
    return MALWARE;
  return (name == kPhishingList) ? PHISH : INVALID;
}
