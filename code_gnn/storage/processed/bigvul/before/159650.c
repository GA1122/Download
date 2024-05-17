void RenderFrameHostManager::OnDidAddContentSecurityPolicies(
    const std::vector<ContentSecurityPolicyHeader>& headers) {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_AddContentSecurityPolicies(
        pair.second->GetRoutingID(), headers));
  }
}
