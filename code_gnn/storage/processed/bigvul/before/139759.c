static std::unique_ptr<TracedValue> UrlForTraceEvent(const KURL& url) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("url", url.GetString());
  return value;
}
