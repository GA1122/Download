bool ContentSecurityPolicy::shouldSendCSPHeader(Resource::Type type) const {
  for (const auto& policy : m_policies) {
    if (policy->shouldSendCSPHeader(type))
      return true;
  }
  return false;
}
