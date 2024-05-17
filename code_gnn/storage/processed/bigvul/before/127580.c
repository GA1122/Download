std::vector<std::string> GetDisplayNames(const std::vector<XID>& output_ids) {
  std::vector<std::string> names;
  for (size_t i = 0; i < output_ids.size(); ++i) {
    std::string display_name;
    if (GetOutputDeviceData(output_ids[i], NULL, NULL, &display_name))
      names.push_back(display_name);
  }
  return names;
}
