  net::HttpResponseInfo* MakeMockResponseInfo() {
    net::HttpResponseInfo* info = new net::HttpResponseInfo;
    info->request_time = base::Time::Now();
    info->response_time = base::Time::Now();
    info->was_cached = false;
    info->headers = new net::HttpResponseHeaders(
        std::string(kMockHeaders, base::size(kMockHeaders)));
    return info;
  }
