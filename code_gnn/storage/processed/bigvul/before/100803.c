bool HttpResponseHeaders::GetTimeValuedHeader(const std::string& name,
                                              Time* result) const {
  std::string value;
  if (!EnumerateHeader(NULL, name, &value))
    return false;

  return Time::FromString(value.c_str(), result);
}
