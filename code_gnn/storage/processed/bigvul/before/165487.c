bool ContentSecurityPolicy::AllowJavaScriptURLs(
    Element* element,
    const String& source,
    const String& context_url,
    const WTF::OrdinalNumber& context_line,
    SecurityViolationReportingPolicy reporting_policy) const {
  Vector<CSPHashValue> csp_hash_values;
  FillInCSPHashValues(source, script_hash_algorithms_used_, &csp_hash_values);

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    is_allowed &= CheckScriptHashAgainstPolicy(csp_hash_values, policy,
                                               InlineType::kAttribute) ||
                  policy->AllowJavaScriptURLs(element, source, context_url,
                                              context_line, reporting_policy);
  }
  return is_allowed;
}
