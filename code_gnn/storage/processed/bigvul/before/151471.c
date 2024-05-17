  void ExpectUpgradeInsecureRequestHeader(const char* input,
                                          WebURLRequest::FrameType frame_type,
                                          bool should_prefer) {
    KURL input_url(kParsedURLString, input);

    ResourceRequest resource_request(input_url);
    resource_request.SetRequestContext(WebURLRequest::kRequestContextScript);
    resource_request.SetFrameType(frame_type);

    fetch_context->ModifyRequestForCSP(resource_request);

    EXPECT_EQ(
        should_prefer ? String("1") : String(),
        resource_request.HttpHeaderField(HTTPNames::Upgrade_Insecure_Requests));

    if (should_prefer) {
      fetch_context->ModifyRequestForCSP(resource_request);
      EXPECT_EQ("1", resource_request.HttpHeaderField(
                         HTTPNames::Upgrade_Insecure_Requests));
    }
  }
