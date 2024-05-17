std::string HttpResponseHeaders::GetStatusLine() const {
  return std::string(raw_headers_.c_str());
}
