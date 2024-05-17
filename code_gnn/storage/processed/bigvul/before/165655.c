std::wstring GetElevationServiceName() {
  std::wstring name = GetElevationServiceDisplayName();
  name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
  return name;
}
