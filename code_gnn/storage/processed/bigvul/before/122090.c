size_t IndexOf(const std::vector<string16>& warnings,
               const std::string& warning) {
  for (size_t i = 0; i < warnings.size(); ++i) {
    if (warnings[i] == ASCIIToUTF16(warning))
      return i;
  }

  return warnings.size();
}
