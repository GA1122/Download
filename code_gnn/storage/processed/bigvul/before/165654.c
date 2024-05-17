std::wstring GetElevationServiceDisplayName() {
  static constexpr wchar_t kElevationServiceDisplayName[] =
      L" Elevation Service";
  return GetBaseAppName() + kElevationServiceDisplayName;
}
