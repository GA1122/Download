void RenderFrameHostManager::OnDidStopLoading() {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_DidStopLoading(pair.second->GetRoutingID()));
  }
}
