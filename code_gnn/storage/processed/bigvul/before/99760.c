inline bool UseLibV4L2() {
#if defined(USE_LIBV4L2)
  return true;
#else
  return false;
#endif
}
