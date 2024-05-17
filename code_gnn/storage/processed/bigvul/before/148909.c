void RenderFrameHostManager::ResetProxyHosts() {
  for (const auto& pair : proxy_hosts_) {
    static_cast<SiteInstanceImpl*>(pair.second->GetSiteInstance())
        ->RemoveObserver(this);
  }
  proxy_hosts_.clear();
}
