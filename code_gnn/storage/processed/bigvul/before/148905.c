void RenderFrameHostManager::OnSetHasReceivedUserGesture() {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(
        new FrameMsg_SetHasReceivedUserGesture(pair.second->GetRoutingID()));
  }
}
