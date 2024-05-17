bool MatchPatternImpl(const std::wstring& source,
                      const std::wstring& pattern,
                      size_t source_index,
                      size_t pattern_index) {
  if (source.empty() && pattern.empty())
    return true;

  if (source_index > source.length() || pattern_index > pattern.length())
    return false;

  if ((source_index == source.length()) &&
      (pattern_index == pattern.length())) {
    return true;
  }

  if ((pattern[pattern_index] == L'*') &&
      (pattern_index + 1 < pattern.length()) &&
      (source_index >= source.length())) {
    return false;
  }

  if ((pattern[pattern_index] == L'?') ||
      (pattern[pattern_index] == source[source_index])) {
    return MatchPatternImpl(source, pattern, source_index + 1,
                            pattern_index + 1);
  }

  if (pattern[pattern_index] == L'*') {
    return MatchPatternImpl(source, pattern, source_index + 1, pattern_index) ||
           MatchPatternImpl(source, pattern, source_index, pattern_index + 1);
  }
  return false;
}
