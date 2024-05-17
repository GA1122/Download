void SuccessfullyBlocked(const wchar_t** blocked_dlls, int* size) {
  if (size == NULL)
    return;

  if (blocked_dlls == NULL && *size < g_num_blocked_dlls) {
    *size = g_num_blocked_dlls;
    return;
  }

  *size = g_num_blocked_dlls;

  int strings_to_fill = 0;
  for (int i = 0; strings_to_fill < g_num_blocked_dlls && g_troublesome_dlls[i];
       ++i) {
    if (g_blocked_dlls[i]) {
      blocked_dlls[strings_to_fill] = g_troublesome_dlls[i];
      ++strings_to_fill;
    }
  }
}
