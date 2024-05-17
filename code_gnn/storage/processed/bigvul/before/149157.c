void OnThreadTermination(void* regex_matcher) {
  delete reinterpret_cast<icu::RegexMatcher*>(regex_matcher);
}
