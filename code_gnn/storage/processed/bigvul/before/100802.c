std::string HttpResponseHeaders::GetStatusText() const {
  std::string status_text = GetStatusLine();
  std::string::const_iterator begin = status_text.begin();
  std::string::const_iterator end = status_text.end();
  for (int i = 0; i < 2; ++i)
    begin = std::find(begin, end, ' ') + 1;
  return std::string(begin, end);
}
