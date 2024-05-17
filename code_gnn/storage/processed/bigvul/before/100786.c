bool HttpResponseHeaders::GetAgeValue(TimeDelta* result) const {
  std::string value;
  if (!EnumerateHeader(NULL, "Age", &value))
    return false;

  int64 seconds;
  base::StringToInt64(value, &seconds);
  *result = TimeDelta::FromSeconds(seconds);
  return true;
}
