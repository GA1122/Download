std::string TestURLLoader::TestTrustedCorbEligibleRequest() {
  std::string cross_origin_url =
      GetReachableCrossOriginURL("corb_eligible_resource.json");

  pp::URLRequestInfo request(instance_);
  request.SetURL(cross_origin_url);
  request.SetAllowCrossOriginRequests(true);

  std::string response_body;
  int32_t rv = OpenTrusted(request, &response_body);
  if (rv != PP_OK)
    return ReportError("Trusted CORB-eligible request failed", rv);

  ASSERT_EQ("{ \"foo\": \"bar\" }\n", response_body);
  PASS();
}
