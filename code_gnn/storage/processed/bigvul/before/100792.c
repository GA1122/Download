bool HttpResponseHeaders::GetDateValue(Time* result) const {
  return GetTimeValuedHeader("Date", result);
}
