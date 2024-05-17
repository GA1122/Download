bool HttpResponseHeaders::GetMaxAgeValue(TimeDelta* result) const {
  std::string name = "cache-control";
  std::string value;

  const char kMaxAgePrefix[] = "max-age=";
  const size_t kMaxAgePrefixLen = arraysize(kMaxAgePrefix) - 1;

  void* iter = NULL;
  while (EnumerateHeader(&iter, name, &value)) {
    if (value.size() > kMaxAgePrefixLen) {
      if (LowerCaseEqualsASCII(value.begin(),
                               value.begin() + kMaxAgePrefixLen,
                               kMaxAgePrefix)) {
        int64 seconds;
        base::StringToInt64(value.begin() + kMaxAgePrefixLen,
                            value.end(),
                            &seconds);
        *result = TimeDelta::FromSeconds(seconds);
        return true;
      }
    }
  }

  return false;
}
