char* do_strdup(const char* src) {
#if defined(OS_WIN)
  return _strdup(src);
#else
  return strdup(src);
#endif
}
