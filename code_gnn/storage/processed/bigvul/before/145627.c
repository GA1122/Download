bool NeedsHTTPOrigin(net::HttpRequestHeaders* headers,
                     const std::string& method) {
  if (headers->HasHeader(net::HttpRequestHeaders::kOrigin))
    return false;

  if (method == "GET" || method == "HEAD")
    return false;

  return true;
}
