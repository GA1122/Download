  std::unique_ptr<net::test_server::HttpResponse> HandleMockResource(
      const net::test_server::HttpRequest& request) {
    auto it =
        num_remaining_requests_to_delay_for_path_.find(request.GetURL().path());
    if (it == num_remaining_requests_to_delay_for_path_.end())
      return nullptr;

    if (it->second > 0) {
      --it->second;
      return std::make_unique<DelayedResponse>(this);
    }
    MaybeStartRequests();
    return std::unique_ptr<net::test_server::BasicHttpResponse>();
  }
