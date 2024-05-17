  void ExpectSetRequiredCSPRequestHeader(
      const char* input,
      WebURLRequest::FrameType frame_type,
      const AtomicString& expected_required_csp) {
    KURL input_url(kParsedURLString, input);
    ResourceRequest resource_request(input_url);
    resource_request.SetRequestContext(WebURLRequest::kRequestContextScript);
    resource_request.SetFrameType(frame_type);

    fetch_context->ModifyRequestForCSP(resource_request);

    EXPECT_EQ(expected_required_csp,
              resource_request.HttpHeaderField(HTTPNames::Sec_Required_CSP));
  }
