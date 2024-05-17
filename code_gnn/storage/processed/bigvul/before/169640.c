std::string TestURLLoader::TestUntrustedHttpRequests() {
  {
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("cOnNeCt", std::string()));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("tRaCk", std::string()));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("tRaCe", std::string()));
    ASSERT_EQ(PP_ERROR_NOACCESS,
        OpenUntrusted("POST\x0d\x0ax-csrf-token:\x20test1234", std::string()));
  }
  {
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Accept-Charset:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Accept-Encoding:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Connection:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Content-Length:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Cookie:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Cookie2:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Date:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Dnt:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Expect:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Host:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Keep-Alive:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Referer:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "TE:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Trailer:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS,
              OpenUntrusted("GET", "Transfer-Encoding:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Upgrade:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "User-Agent:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Via:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted(
        "GET", "Proxy-Authorization: Basic dXNlcjpwYXNzd29yZA==:\n"));
    ASSERT_EQ(PP_ERROR_NOACCESS, OpenUntrusted("GET", "Sec-foo:\n"));
  }
  {
    pp::URLRequestInfo request(instance_);
    request.SetCustomReferrerURL("http://www.google.com/");

    int32_t rv = OpenUntrusted(request, NULL);
    if (rv != PP_ERROR_NOACCESS)
      return ReportError(
          "Untrusted request with custom referrer restriction", rv);
  }
  {
    pp::URLRequestInfo request(instance_);
    request.SetCustomContentTransferEncoding("foo");

    int32_t rv = OpenUntrusted(request, NULL);
    if (rv != PP_ERROR_NOACCESS)
      return ReportError(
          "Untrusted request with content-transfer-encoding restriction", rv);
  }

  PASS();
}
