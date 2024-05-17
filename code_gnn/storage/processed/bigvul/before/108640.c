static bool SystemSupportsOCSP() {
#if defined(USE_OPENSSL)
  return false;
#elif defined(OS_WIN)
  return base::win::GetVersion() >= base::win::VERSION_VISTA;
#elif defined(OS_ANDROID)
  return false;
#else
  return true;
#endif
}
