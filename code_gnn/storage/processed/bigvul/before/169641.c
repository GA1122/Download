std::string TestURLLoader::TestUntrustedSameOriginRestriction() {
  pp::URLRequestInfo request(instance_);
  std::string cross_origin_url = GetReachableCrossOriginURL("test_case.html");
  request.SetURL(cross_origin_url);

  int32_t rv = OpenUntrusted(request, NULL);
  if (rv != PP_ERROR_NOACCESS)
    return ReportError(
        "Untrusted, unintended cross-origin request restriction", rv);

  PASS();
}
