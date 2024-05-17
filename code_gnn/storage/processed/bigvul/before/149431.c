void ContentSecurityPolicy::didReceiveHeaders(
    const ContentSecurityPolicyResponseHeaders& headers) {
  if (!headers.contentSecurityPolicy().isEmpty())
    addAndReportPolicyFromHeaderValue(headers.contentSecurityPolicy(),
                                      ContentSecurityPolicyHeaderTypeEnforce,
                                      ContentSecurityPolicyHeaderSourceHTTP);
  if (!headers.contentSecurityPolicyReportOnly().isEmpty())
    addAndReportPolicyFromHeaderValue(headers.contentSecurityPolicyReportOnly(),
                                      ContentSecurityPolicyHeaderTypeReport,
                                      ContentSecurityPolicyHeaderSourceHTTP);
}
