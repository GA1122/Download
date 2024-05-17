bool ContentSecurityPolicy::IsFrameAncestorsEnforced() const {
  for (const auto& policy : policies_) {
    if (policy->IsFrameAncestorsEnforced())
      return true;
  }
  return false;
}
