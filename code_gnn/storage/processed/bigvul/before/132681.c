void BlinkTestRunner::SetLocale(const std::string& locale) {
  setlocale(LC_ALL, locale.c_str());
}
