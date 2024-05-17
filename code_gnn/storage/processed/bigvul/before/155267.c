void SetJavaExceptionInfo(const char* info_string) {
  static crashpad::StringAnnotation<5 * 4096> exception_info("exception_info");
  if (info_string) {
    exception_info.Set(info_string);
  } else {
    exception_info.Clear();
  }
}
