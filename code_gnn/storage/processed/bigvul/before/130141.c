bool AddDllToBlacklist(const wchar_t* dll_name) {
  int blacklist_size = BlacklistSize();
  if (blacklist_size + 1 >= kTroublesomeDllsMaxCount)
    return false;
  for (int i = 0; i < blacklist_size; ++i) {
    if (!_wcsicmp(g_troublesome_dlls[i], dll_name))
      return true;
  }

  wchar_t* str_buffer = new wchar_t[wcslen(dll_name) + 1];
  wcscpy(str_buffer, dll_name);

  g_troublesome_dlls[blacklist_size] = str_buffer;
  g_blocked_dlls[blacklist_size] = false;
  return true;
}
