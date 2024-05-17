void RenderFrameHostManager::OnDidResetContentSecurityPolicy() {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(
        new FrameMsg_ResetContentSecurityPolicy(pair.second->GetRoutingID()));
  }
}
