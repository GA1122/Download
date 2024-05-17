  void MonitorAndVerifyRequestsToProxyServer(
      const net::test_server::HttpRequest& request) {
    ++count_proxy_server_requests_received_;
    EXPECT_NE(request.headers.end(),
              request.headers.find(data_reduction_proxy::chrome_proxy_header()))
        << " url=" << request.GetURL() << " path=" << request.GetURL().path();

    VerifyChromeProxyRequestHeader(
        request.headers.at(data_reduction_proxy::chrome_proxy_header()));

    EXPECT_NE(
        request.headers.end(),
        request.headers.find(data_reduction_proxy::chrome_proxy_ect_header()))
        << " url=" << request.GetURL() << " path=" << request.GetURL().path();

    base::AutoLock lock(lock_);
    monitored_urls_.push_back(request.GetURL());
  }
