bool ShouldSaveOrRestoreWindowPos() {
#if defined(OS_WIN) && !defined(USE_AURA)
  if (base::win::IsMetroProcess())
    return false;
#endif
  return true;
}
