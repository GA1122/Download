  void ExpectHeader(const char* input,
                    const char* header_name,
                    bool is_present,
                    const char* header_value,
                    float width = 0) {
    ClientHintsPreferences hints_preferences;

    FetchParameters::ResourceWidth resource_width;
    if (width > 0) {
      resource_width.width = width;
      resource_width.is_set = true;
    }

    KURL input_url(kParsedURLString, input);
    ResourceRequest resource_request(input_url);

    fetch_context->AddClientHintsIfNecessary(hints_preferences, resource_width,
                                             resource_request);

    EXPECT_EQ(is_present ? String(header_value) : String(),
              resource_request.HttpHeaderField(header_name));
  }
