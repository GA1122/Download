void BlockedDll(size_t blocked_index) {
  assert(blocked_index < kTroublesomeDllsMaxCount);

  if (!g_blocked_dlls[blocked_index] &&
      blocked_index < kTroublesomeDllsMaxCount) {
    ++g_num_blocked_dlls;
    g_blocked_dlls[blocked_index] = true;
  }
}
