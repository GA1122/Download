bool HttpResponseHeaders::GetExpiresValue(Time* result) const {
  return GetTimeValuedHeader("Expires", result);
}
