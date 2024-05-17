void ContentSecurityPolicy::reportAccumulatedHeaders(
    LocalFrameClient* client) const {
  DCHECK(client);
  for (const auto& policy : m_policies) {
    client->didAddContentSecurityPolicy(
        policy->header(), policy->headerType(), policy->headerSource(),
        {policy->exposeForNavigationalChecks()});
  }
}
