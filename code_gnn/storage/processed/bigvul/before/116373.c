void Verify206Response(std::string response, int start, int end) {
  std::string raw_headers(net::HttpUtil::AssembleRawHeaders(response.data(),
                                                            response.size()));
  scoped_refptr<net::HttpResponseHeaders> headers(
      new net::HttpResponseHeaders(raw_headers));

  ASSERT_EQ(206, headers->response_code());

  int64 range_start, range_end, object_size;
  ASSERT_TRUE(
      headers->GetContentRange(&range_start, &range_end, &object_size));
  int64 content_length = headers->GetContentLength();

  int length = end - start + 1;
  ASSERT_EQ(length, content_length);
  ASSERT_EQ(start, range_start);
  ASSERT_EQ(end, range_end);
}
