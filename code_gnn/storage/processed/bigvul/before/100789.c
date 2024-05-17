int64 HttpResponseHeaders::GetContentLength() const {
  void* iter = NULL;
  std::string content_length_val;
  if (!EnumerateHeader(&iter, "content-length", &content_length_val))
    return -1;

  if (content_length_val.empty())
    return -1;

  if (content_length_val[0] == '+')
    return -1;

  int64 result;
  bool ok = base::StringToInt64(content_length_val, &result);
  if (!ok || result < 0)
    return -1;

  return result;
}
