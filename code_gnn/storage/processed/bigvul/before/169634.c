std::string TestURLLoader::TestTrustedHttpRequests() {
  {
    ASSERT_EQ(PP_OK, OpenTrusted("cOnNeCt", std::string()));
    ASSERT_EQ(PP_OK, OpenTrusted("tRaCk", std::string()));
    ASSERT_EQ(PP_OK, OpenTrusted("tRaCe", std::string()));
  }
  {
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Accept-Charset:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Accept-Encoding:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Connection:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Content-Length:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Cookie:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Cookie2:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Content-Transfer-Encoding:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Date:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Expect:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Host:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Keep-Alive:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Referer:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "TE:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Trailer:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Transfer-Encoding:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Upgrade:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "User-Agent:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Via:\n"));
    ASSERT_EQ(PP_OK,
              OpenTrusted("GET",
                  "Proxy-Authorization: Basic dXNlcjpwYXNzd29yZA==:\n"));
    ASSERT_EQ(PP_OK, OpenTrusted("GET", "Sec-foo:\n"));
  }
  {
    pp::URLRequestInfo request(instance_);
    request.SetCustomReferrerURL("http://www.google.com/");

    int32_t rv = OpenTrusted(request, NULL);
    if (rv != PP_OK)
      return ReportError("Trusted request with custom referrer", rv);
  }
  {
    pp::URLRequestInfo request(instance_);
    request.SetCustomContentTransferEncoding("foo");

    int32_t rv = OpenTrusted(request, NULL);
    if (rv != PP_OK)
      return ReportError(
          "Trusted request with content-transfer-encoding failed", rv);
  }

  PASS();
}
