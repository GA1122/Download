void ContentSecurityPolicy::ReportAccumulatedHeaders(
    LocalFrameClient* client) const {
  DCHECK(client);
  WebVector<WebContentSecurityPolicy> policies(policies_.size());
  for (wtf_size_t i = 0; i < policies_.size(); ++i)
    policies[i] = policies_[i]->ExposeForNavigationalChecks();
  client->DidAddContentSecurityPolicies(policies);
}
