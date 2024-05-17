int Instance::GetInitialPage(const std::string& url) {
  size_t found_idx = url.find('#');
  if (found_idx == std::string::npos)
    return -1;

  const std::string& ref = url.substr(found_idx + 1);
  std::vector<std::string> fragments;
  Tokenize(ref, kDelimiters, &fragments);

  int page = -1;

  if ((fragments.size() == 1) && (fragments[0].find('=') == std::string::npos))
    return engine_->GetNamedDestinationPage(fragments[0]);

  for (size_t i = 0; i < fragments.size(); ++i) {
    std::vector<std::string> key_value;
    base::SplitString(fragments[i], '=', &key_value);
    if (key_value.size() != 2)
      continue;
    const std::string& key = key_value[0];
    const std::string& value = key_value[1];

    if (base::strcasecmp(kPage, key.c_str()) == 0) {
      int page_value = -1;
      if (base::StringToInt(value, &page_value) && page_value > 0)
        page = page_value - 1;
      continue;
    }
    if (base::strcasecmp(kNamedDest, key.c_str()) == 0) {
      int page_value = engine_->GetNamedDestinationPage(value);
      if (page_value >= 0)
        page = page_value;
      continue;
    }
  }
  return page;
}
