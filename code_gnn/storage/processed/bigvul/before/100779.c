void HttpResponseHeaders::AddHopByHopHeaders(HeaderSet* result) {
  for (size_t i = 0; i < arraysize(kHopByHopResponseHeaders); ++i)
    result->insert(std::string(kHopByHopResponseHeaders[i]));
}
