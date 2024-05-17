bool RemoveDllFromBlacklist(const wchar_t* dll_name) {
  int blacklist_size = BlacklistSize();
  for (int i = 0; i < blacklist_size; ++i) {
    if (!_wcsicmp(g_troublesome_dlls[i], dll_name)) {
      delete[] g_troublesome_dlls[i];
      g_troublesome_dlls[i] = g_troublesome_dlls[blacklist_size - 1];
      g_troublesome_dlls[blacklist_size - 1] = NULL;

      if (g_blocked_dlls[i])
        --g_num_blocked_dlls;
      g_blocked_dlls[i] = g_blocked_dlls[blacklist_size - 1];
      return true;
    }
  }
  return false;
}
