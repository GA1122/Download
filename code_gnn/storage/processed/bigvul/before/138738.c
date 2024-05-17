void RenderFrameHostImpl::OnDidAddContentSecurityPolicies(
    const std::vector<ContentSecurityPolicy>& policies) {
  std::vector<ContentSecurityPolicyHeader> headers;
  for (const ContentSecurityPolicy& policy : policies) {
    AddContentSecurityPolicy(policy);
    headers.push_back(policy.header);
  }
  frame_tree_node()->AddContentSecurityPolicies(headers);
}
