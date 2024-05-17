std::wstring GetCrashDumpLocation() {
  std::wstring user_data_dir;
  bool ret = GetUserDataDirectory(&user_data_dir, nullptr);
  assert(ret);
  IgnoreUnused(ret);
  return user_data_dir.append(L"\\Crashpad");
}
