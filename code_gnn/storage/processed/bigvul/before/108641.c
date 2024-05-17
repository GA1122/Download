static bool SystemUsesChromiumEVMetadata() {
#if defined(USE_OPENSSL)
  return false;
#elif defined(OS_MACOSX)
  return false;
#else
  return true;
#endif
}
