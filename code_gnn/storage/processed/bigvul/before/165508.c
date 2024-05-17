void ContentSecurityPolicy::DidReceiveHeaders(
    const ContentSecurityPolicyResponseHeaders& headers) {
  if (headers.ShouldParseWasmEval()) {
    supports_wasm_eval_ = true;
  }
  if (!headers.ContentSecurityPolicy().IsEmpty())
    AddAndReportPolicyFromHeaderValue(headers.ContentSecurityPolicy(),
                                      kContentSecurityPolicyHeaderTypeEnforce,
                                      kContentSecurityPolicyHeaderSourceHTTP);
  if (!headers.ContentSecurityPolicyReportOnly().IsEmpty())
    AddAndReportPolicyFromHeaderValue(headers.ContentSecurityPolicyReportOnly(),
                                      kContentSecurityPolicyHeaderTypeReport,
                                      kContentSecurityPolicyHeaderSourceHTTP);
}
