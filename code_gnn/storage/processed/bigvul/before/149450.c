bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    const CSPHashValue& hashValue,
                    ContentSecurityPolicy::InlineType type) {
  bool isAllowed = true;
  for (const auto& policy : policies)
    isAllowed &= (policy.get()->*allowed)(hashValue, type);
  return isAllowed;
}
