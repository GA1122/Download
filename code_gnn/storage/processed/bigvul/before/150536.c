  std::unique_ptr<net::test_server::HttpResponse> WaitForWarmupRequest(
      base::RunLoop* run_loop,
      const net::test_server::HttpRequest& request) {
    auto response = std::make_unique<net::test_server::BasicHttpResponse>();
    if (base::StartsWith(request.relative_url, "/e2e_probe",
                         base::CompareCase::SENSITIVE)) {
      run_loop->Quit();
      response->set_content("content");
      response->AddCustomHeader("via", via_header_);
      const auto user_agent =
          request.headers.find(net::HttpRequestHeaders::kUserAgent);
      EXPECT_TRUE(user_agent != request.headers.end());
      EXPECT_THAT(user_agent->second, HasSubstr("Chrome/"));
    } else if (base::StartsWith(request.relative_url, "/echoheader",
                                base::CompareCase::SENSITIVE)) {
      const auto chrome_proxy_header = request.headers.find("chrome-proxy");
      if (chrome_proxy_header != request.headers.end()) {
        response->set_content(chrome_proxy_header->second);
        response->AddCustomHeader("chrome-proxy", "ofcl=1000");
      }
    }
    return response;
  }
