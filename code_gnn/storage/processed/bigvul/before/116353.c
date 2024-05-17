static void ConditionalizedRequestUpdatesCacheHelper(
    const Response& net_response_1,
    const Response& net_response_2,
    const Response& cached_response_2,
    const char* extra_request_headers) {
  MockHttpCache cache;

  const char* kUrl = "http://foobar.com/main.css";

  static const Response kUnexpectedResponse = {
    "HTTP/1.1 500 Unexpected",
    "Server: unexpected_header",
    "unexpected body"
  };

  MockTransaction mock_network_response = { 0 };
  mock_network_response.url = kUrl;
  AddMockTransaction(&mock_network_response);


  MockTransaction request = { 0 };
  request.url = kUrl;
  request.method = "GET";
  request.request_headers = "";

  net_response_1.AssignTo(&mock_network_response);   
  net_response_1.AssignTo(&request);                 

  std::string response_headers;
  RunTransactionTestWithResponse(
      cache.http_cache(), request, &response_headers);

  EXPECT_EQ(net_response_1.status_and_headers(), response_headers);
  EXPECT_EQ(1, cache.network_layer()->transaction_count());
  EXPECT_EQ(0, cache.disk_cache()->open_count());
  EXPECT_EQ(1, cache.disk_cache()->create_count());


  request.load_flags = net::LOAD_ONLY_FROM_CACHE;

  kUnexpectedResponse.AssignTo(&mock_network_response);   
  net_response_1.AssignTo(&request);                      

  RunTransactionTestWithResponse(
      cache.http_cache(), request, &response_headers);

  EXPECT_EQ(net_response_1.status_and_headers(), response_headers);
  EXPECT_EQ(1, cache.network_layer()->transaction_count());
  EXPECT_EQ(1, cache.disk_cache()->open_count());
  EXPECT_EQ(1, cache.disk_cache()->create_count());


  request.request_headers = extra_request_headers;
  request.load_flags = net::LOAD_NORMAL;

  net_response_2.AssignTo(&mock_network_response);   
  net_response_2.AssignTo(&request);                 

  RunTransactionTestWithResponse(
      cache.http_cache(), request, &response_headers);

  EXPECT_EQ(net_response_2.status_and_headers(), response_headers);
  EXPECT_EQ(2, cache.network_layer()->transaction_count());
  EXPECT_EQ(1, cache.disk_cache()->open_count());
  EXPECT_EQ(1, cache.disk_cache()->create_count());


  request.request_headers = "";
  request.load_flags = net::LOAD_ONLY_FROM_CACHE;

  kUnexpectedResponse.AssignTo(&mock_network_response);   
  cached_response_2.AssignTo(&request);                   

  RunTransactionTestWithResponse(
      cache.http_cache(), request, &response_headers);

  EXPECT_EQ(cached_response_2.status_and_headers(), response_headers);
  EXPECT_EQ(2, cache.network_layer()->transaction_count());
  EXPECT_EQ(2, cache.disk_cache()->open_count());
  EXPECT_EQ(1, cache.disk_cache()->create_count());

  RemoveMockTransaction(&mock_network_response);
}
