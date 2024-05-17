inline bool IsOzone() {
#if defined(USE_OZONE)
  return true;
#else
  return false;
#endif
}
