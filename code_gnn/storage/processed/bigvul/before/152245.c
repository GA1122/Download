void RenderFrameImpl::DidAddContentSecurityPolicies(
    const blink::WebVector<blink::WebContentSecurityPolicy>& policies) {
  std::vector<ContentSecurityPolicy> content_policies;
  for (const auto& policy : policies)
    content_policies.push_back(BuildContentSecurityPolicy(policy));

  Send(new FrameHostMsg_DidAddContentSecurityPolicies(routing_id_,
                                                      content_policies));
}
