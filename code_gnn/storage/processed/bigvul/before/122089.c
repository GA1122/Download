bool Contains(const std::vector<string16>& warnings,
              const std::string& warning) {
  return IndexOf(warnings, warning) != warnings.size();
}
