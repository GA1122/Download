bool IsControlKeyModifier(int flags) {
#if defined(OS_LINUX)
  return flags & ui::EF_CONTROL_DOWN;
#else
  return false;
#endif
}
