void HttpResponseHeaders::AddChallengeHeaders(HeaderSet* result) {
  for (size_t i = 0; i < arraysize(kChallengeResponseHeaders); ++i)
    result->insert(std::string(kChallengeResponseHeaders[i]));
}
