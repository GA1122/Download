bool ContentSecurityPolicy::AllowInlineScript(
    Element* element,
    const String& context_url,
    const String& nonce,
    const WTF::OrdinalNumber& context_line,
    const String& script_content,
    InlineType inline_type,
    SecurityViolationReportingPolicy reporting_policy) const {
  DCHECK(element);

  Vector<CSPHashValue> csp_hash_values;
  FillInCSPHashValues(script_content, script_hash_algorithms_used_,
                      &csp_hash_values);

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    is_allowed &=
        CheckScriptHashAgainstPolicy(csp_hash_values, policy, inline_type) ||
        policy->AllowInlineScript(element, context_url, nonce, context_line,
                                  reporting_policy, script_content);
  }

  return is_allowed;
}
