std::wstring GetChromeInstallSubDirectory() {
  std::wstring result;
  AppendChromeInstallSubDirectory(InstallDetails::Get().mode(),
                                  true  , &result);
  return result;
}
