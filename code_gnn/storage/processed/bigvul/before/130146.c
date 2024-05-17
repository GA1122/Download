int GetBlacklistIndex(const wchar_t* dll_name) {
  for (int i = 0; i < kTroublesomeDllsMaxCount && g_troublesome_dlls[i]; ++i) {
    if (_wcsicmp(dll_name, g_troublesome_dlls[i]) == 0)
      return i;
  }
  return -1;
}
