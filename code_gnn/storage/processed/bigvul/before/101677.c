std::string Browser::GetWindowPlacementKey() const {
  std::string name(prefs::kBrowserWindowPlacement);
  if (!app_name_.empty()) {
    name.append("_");
    name.append(app_name_);
  }
  return name;
}
