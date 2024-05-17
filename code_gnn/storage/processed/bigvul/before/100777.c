void HttpResponseHeaders::AddCookieHeaders(HeaderSet* result) {
  for (size_t i = 0; i < arraysize(kCookieResponseHeaders); ++i)
    result->insert(std::string(kCookieResponseHeaders[i]));
}
