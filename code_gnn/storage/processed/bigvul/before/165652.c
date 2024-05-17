bool GetCollectStatsInSample() {
  std::wstring registry_path = GetRegistryPath();

  DWORD out_value = 0;
  if (!nt::QueryRegValueDWORD(nt::HKCU, nt::WOW6432, registry_path.c_str(),
                              kRegValueChromeStatsSample, &out_value)) {
    return true;
  }
  return out_value == 1;
}
