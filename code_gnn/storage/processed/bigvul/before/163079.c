struct tm* localtime64_r_override(const time_t* timep, struct tm* result) {
  if (g_am_zygote_or_renderer) {
    ProxyLocaltimeCallToBrowser(*timep, result, NULL, 0);
    return result;
  }

  CHECK_EQ(0, pthread_once(&g_libc_localtime_funcs_guard,
                           InitLibcLocaltimeFunctions));
  struct tm* res = g_libc_localtime64_r(timep, result);
#if defined(MEMORY_SANITIZER)
  if (res) __msan_unpoison(res, sizeof(*res));
  if (res->tm_zone) __msan_unpoison_string(res->tm_zone);
#endif
  return res;
}
