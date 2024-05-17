bool Matches(const URLMatcherCondition& condition, const std::string& text) {
  return text.find(condition.string_pattern()->pattern()) !=
      std::string::npos;
}
