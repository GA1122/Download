void RenderFrameHostManager::UpdateUserActivationState(
    blink::UserActivationUpdateType update_type) {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_UpdateUserActivationState(
        pair.second->GetRoutingID(), update_type));
  }
}
