bool ContentSecurityPolicy::AllowScriptFromSource(
    const KURL& url,
    const String& nonce,
    const IntegrityMetadataSet& hashes,
    ParserDisposition parser_disposition,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy,
    CheckHeaderType check_header_type) const {
  if (ShouldBypassContentSecurityPolicy(url, execution_context_)) {
    UseCounter::Count(
        GetDocument(),
        parser_disposition == kParserInserted
            ? WebFeature::kScriptWithCSPBypassingSchemeParserInserted
            : WebFeature::kScriptWithCSPBypassingSchemeNotParserInserted);

    if ((parser_disposition == kNotParserInserted ||
         !ExperimentalFeaturesEnabled()) &&
        !SchemeRegistry::ShouldTreatURLSchemeAsNotAllowingJavascriptURLs(
            execution_context_->GetSecurityOrigin()->Protocol())) {
      return true;
    }
  }

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    if (!CheckHeaderTypeMatches(check_header_type, policy->HeaderType()))
      continue;
    is_allowed &=
        policy->AllowScriptFromSource(url, nonce, hashes, parser_disposition,
                                      redirect_status, reporting_policy);
  }
  return is_allowed;
}
