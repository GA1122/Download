void RenderFrameHostManager::OnEnforceInsecureRequestPolicy(
    blink::WebInsecureRequestPolicy policy) {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_EnforceInsecureRequestPolicy(
        pair.second->GetRoutingID(), policy));
  }
}
