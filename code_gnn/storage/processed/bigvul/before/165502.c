bool ContentSecurityPolicy::CheckScriptHashAgainstPolicy(
    Vector<CSPHashValue>& csp_hash_values,
    const Member<CSPDirectiveList>& policy,
    InlineType inline_type) {
  for (const auto& csp_hash_value : csp_hash_values) {
    if (policy->AllowScriptHash(csp_hash_value, inline_type)) {
      return true;
    }
  }
  return false;
}
