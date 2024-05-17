bool PreProcessName(const std::wstring& path, std::wstring* new_path) {
  ConvertToLongPath(path, new_path);

  bool reparsed = false;
  if (ERROR_SUCCESS != IsReparsePoint(*new_path, &reparsed))
    return false;

  return !reparsed;
}
