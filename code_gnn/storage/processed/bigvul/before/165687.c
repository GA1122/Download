bool SetCollectStatsInSample(bool in_sample) {
  std::wstring registry_path = GetRegistryPath();

  HANDLE key_handle = INVALID_HANDLE_VALUE;
  if (!nt::CreateRegKey(nt::HKCU, registry_path.c_str(),
                        KEY_SET_VALUE | KEY_WOW64_32KEY, &key_handle)) {
    return false;
  }

  bool success = nt::SetRegValueDWORD(key_handle, kRegValueChromeStatsSample,
                                      in_sample ? 1 : 0);
  nt::CloseRegKey(key_handle);
  return success;
}
