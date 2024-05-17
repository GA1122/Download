void RenderFrameHostManager::OnSetHasReceivedUserGestureBeforeNavigation(
    bool value) {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_SetHasReceivedUserGestureBeforeNavigation(
        pair.second->GetRoutingID(), value));
  }
}
