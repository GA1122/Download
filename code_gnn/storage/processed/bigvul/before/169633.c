std::string TestURLLoader::TestTrustedCrossOriginRequest() {
  pp::URLRequestInfo request(instance_);
  std::string cross_origin_url = GetReachableCrossOriginURL("test_case.html");
  request.SetURL(cross_origin_url);
  request.SetAllowCrossOriginRequests(true);

  int32_t rv = OpenTrusted(request, NULL);
  if (rv != PP_OK)
    return ReportError("Trusted cross-origin request failed", rv);

  PASS();
}
