bool HttpResponseHeaders::HasHeader(const std::string& name) const {
  return FindHeader(0, name) != std::string::npos;
}
