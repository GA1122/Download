std::string TestURLLoader::TestUntrustedCorbEligibleRequest() {
  std::string cross_origin_url =
      GetReachableCrossOriginURL("corb_eligible_resource.json");

  pp::URLRequestInfo request(instance_);
  request.SetURL(cross_origin_url);
  request.SetAllowCrossOriginRequests(true);

  std::string response_body;
  int32_t rv = OpenUntrusted(request, &response_body);

  if (rv != PP_ERROR_NOACCESS) {
    return ReportError("Untrusted Javascript URL request restriction failed",
                       rv);
  }
  ASSERT_EQ("", response_body);
  PASS();
}
