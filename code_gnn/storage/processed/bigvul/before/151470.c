  void ExpectUpgrade(const char* input,
                     WebURLRequest::RequestContext request_context,
                     WebURLRequest::FrameType frame_type,
                     const char* expected) {
    KURL input_url(kParsedURLString, input);
    KURL expected_url(kParsedURLString, expected);

    ResourceRequest resource_request(input_url);
    resource_request.SetRequestContext(request_context);
    resource_request.SetFrameType(frame_type);

    fetch_context->ModifyRequestForCSP(resource_request);

    EXPECT_EQ(expected_url.GetString(), resource_request.Url().GetString());
    EXPECT_EQ(expected_url.Protocol(), resource_request.Url().Protocol());
    EXPECT_EQ(expected_url.Host(), resource_request.Url().Host());
    EXPECT_EQ(expected_url.Port(), resource_request.Url().Port());
    EXPECT_EQ(expected_url.HasPort(), resource_request.Url().HasPort());
    EXPECT_EQ(expected_url.GetPath(), resource_request.Url().GetPath());
  }
