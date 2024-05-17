std::string TestURLLoader::TestTrustedJavascriptURLRestriction() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("javascript:foo = bar");

  int32_t rv = OpenTrusted(request, NULL);
  if (rv == PP_ERROR_NOACCESS)
  return ReportError(
      "Trusted Javascript URL request", rv);

  PASS();
}
