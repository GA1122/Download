bool ContentSecurityPolicy::IsActiveForConnections() const {
  for (const auto& policy : policies_) {
    if (policy->IsActiveForConnections())
      return true;
  }
  return false;
}
