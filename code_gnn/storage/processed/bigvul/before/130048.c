std::string GetPlatformString() {
#if defined(OS_WIN)
  return "win";
#elif defined(OS_IOS)
  return "ios";
#elif defined(OS_MACOSX)
  return "mac";
#elif defined(OS_CHROMEOS)
  return "chromeos";
#elif defined(OS_ANDROID)
  return "android";
#elif defined(OS_LINUX) || defined(OS_BSD) || defined(OS_SOLARIS)
  return "linux";
#else
#error Unknown platform
#endif
}
