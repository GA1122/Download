bool MatchPattern(const std::wstring& source, const std::wstring& pattern) {
  assert(pattern.find(L"**") == std::wstring::npos);
  return MatchPatternImpl(source, pattern, 0, 0);
}
