std::wstring& AppendChromeInstallSubDirectory(const InstallConstants& mode,
                                              bool include_suffix,
                                              std::wstring* path) {
  if (*kCompanyPathName) {
    path->append(kCompanyPathName);
    path->push_back(L'\\');
  }
  path->append(kProductPathName, kProductPathNameLength);
  if (!include_suffix)
    return *path;
  return path->append(mode.install_suffix);
}
