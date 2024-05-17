bool ShouldUpdateHeader(const std::string::const_iterator& name_begin,
                        const std::string::const_iterator& name_end) {
  for (size_t i = 0; i < arraysize(kNonUpdatedHeaders); ++i) {
    if (LowerCaseEqualsASCII(name_begin, name_end, kNonUpdatedHeaders[i]))
      return false;
  }
  return true;
}
