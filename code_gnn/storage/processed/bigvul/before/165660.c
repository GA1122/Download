void GetExecutableVersionDetails(const std::wstring& exe_path,
                                 std::wstring* product_name,
                                 std::wstring* version,
                                 std::wstring* special_build,
                                 std::wstring* channel_name) {
  assert(product_name);
  assert(version);
  assert(special_build);
  assert(channel_name);

  *product_name = L"Chrome";
  *version = L"0.0.0.0-devel";
  special_build->clear();

  DWORD dummy = 0;
  DWORD length = ::GetFileVersionInfoSize(exe_path.c_str(), &dummy);
  if (length) {
    std::unique_ptr<char[]> data(new char[length]);
    if (::GetFileVersionInfo(exe_path.c_str(), dummy, length, data.get())) {
      GetValueFromVersionResource(data.get(), L"ProductVersion", version);

      std::wstring official_build;
      GetValueFromVersionResource(data.get(), L"Official Build",
                                  &official_build);
      if (official_build != L"1")
        version->append(L"-devel");
      GetValueFromVersionResource(data.get(), L"ProductShortName",
                                  product_name);
      GetValueFromVersionResource(data.get(), L"SpecialBuild", special_build);
    }
  }
  *channel_name = GetChromeChannelName();
}
