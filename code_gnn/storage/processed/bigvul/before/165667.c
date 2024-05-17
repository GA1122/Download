std::wstring GetRegistryPath() {
  std::wstring result(L"Software\\");
  AppendChromeInstallSubDirectory(InstallDetails::Get().mode(),
                                  true  , &result);
  return result;
}
