static void ETagGet_UnconditionalRequest_Handler(
    const net::HttpRequestInfo* request,
    std::string* response_status,
    std::string* response_headers,
    std::string* response_data) {
  EXPECT_FALSE(
      request->extra_headers.HasHeader(net::HttpRequestHeaders::kIfNoneMatch));
}
