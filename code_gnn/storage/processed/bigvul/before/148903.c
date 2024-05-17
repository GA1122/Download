void RenderFrameHostManager::OnDidUpdateOrigin(
    const url::Origin& origin,
    bool is_potentially_trustworthy_unique_origin) {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(
        new FrameMsg_DidUpdateOrigin(pair.second->GetRoutingID(), origin,
                                     is_potentially_trustworthy_unique_origin));
  }
}
