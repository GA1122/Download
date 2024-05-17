scoped_refptr<net::HttpResponseHeaders> SimulateServerResponse(
    int response_code,
    net::TestURLFetcher* fetcher) {
  EXPECT_TRUE(fetcher);
  scoped_refptr<net::HttpResponseHeaders> headers(
      new net::HttpResponseHeaders("date:Wed, 13 Feb 2013 00:25:24 GMT\0\0"));
  fetcher->set_response_headers(headers);
  fetcher->set_response_code(response_code);
  return headers;
}
