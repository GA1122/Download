std::string GetHeaderValue(const net::HttpResponseHeaders* headers,
                           const base::StringPiece& name) {
  std::string value;
  headers->EnumerateHeader(NULL, name, &value);
  return value;
}
