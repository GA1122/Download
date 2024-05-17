  void HTTPRedirectMethodTest(const GURL& redirect_url,
                              const std::string& request_method,
                              const std::string& redirect_method,
                              bool include_data) {
    static const char kData[] = "hello world";
    TestDelegate d;
    URLRequest req(redirect_url, &d, &default_context_);
    req.set_method(request_method);
    if (include_data) {
      req.set_upload(CreateSimpleUploadData(kData).get());
      HttpRequestHeaders headers;
      headers.SetHeader(HttpRequestHeaders::kContentLength,
                        base::UintToString(arraysize(kData) - 1));
      req.SetExtraRequestHeaders(headers);
    }
    req.Start();
    MessageLoop::current()->Run();
    EXPECT_EQ(redirect_method, req.method());
    EXPECT_EQ(URLRequestStatus::SUCCESS, req.status().status());
    EXPECT_EQ(OK, req.status().error());
    if (include_data) {
      if (request_method == redirect_method) {
        EXPECT_EQ(kData, d.data_received());
      } else {
        EXPECT_NE(kData, d.data_received());
      }
    }
    if (HasFailure())
      LOG(WARNING) << "Request method was: " << request_method;
  }
