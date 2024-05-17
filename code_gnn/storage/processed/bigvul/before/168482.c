  bool CheckAccessControlHeaders(const char* header_name, bool exposed) {
    std::string id("http://www.other.com/CheckAccessControlExposeHeaders_");
    id.append(header_name);
    if (exposed)
      id.append("-Exposed");
    id.append(".html");

    KURL url = ToKURL(id);
    WebURLRequest request(url);
    request.SetFetchRequestMode(network::mojom::FetchRequestMode::kCORS);
    request.SetFetchCredentialsMode(
        network::mojom::FetchCredentialsMode::kOmit);

    WebString header_name_string(WebString::FromUTF8(header_name));
    expected_response_ = WebURLResponse();
    expected_response_.SetMIMEType("text/html");
    expected_response_.SetHTTPStatusCode(200);
    expected_response_.AddHTTPHeaderField("Access-Control-Allow-Origin", "*");
    if (exposed) {
      expected_response_.AddHTTPHeaderField("access-control-expose-headers",
                                            header_name_string);
    }
    expected_response_.AddHTTPHeaderField(header_name_string, "foo");
    Platform::Current()->GetURLLoaderMockFactory()->RegisterURL(
        url, expected_response_, frame_file_path_);

    WebAssociatedURLLoaderOptions options;
    expected_loader_ = CreateAssociatedURLLoader(options);
    EXPECT_TRUE(expected_loader_);
    expected_loader_->LoadAsynchronously(request, this);
    ServeRequests();
    EXPECT_TRUE(did_receive_response_);
    EXPECT_TRUE(did_receive_data_);
    EXPECT_TRUE(did_finish_loading_);

    return !actual_response_.HttpHeaderField(header_name_string).IsEmpty();
  }
