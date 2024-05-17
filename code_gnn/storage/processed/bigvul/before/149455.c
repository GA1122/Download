bool ContentSecurityPolicy::isFrameAncestorsEnforced() const {
  for (const auto& policy : m_policies) {
    if (policy->isFrameAncestorsEnforced())
      return true;
  }
  return false;
}
