RenderFrameProxyHost* RenderFrameHostManager::CreateRenderFrameProxyHost(
    SiteInstance* site_instance,
    RenderViewHostImpl* rvh) {
  int site_instance_id = site_instance->GetId();
  CHECK(proxy_hosts_.find(site_instance_id) == proxy_hosts_.end())
      << "A proxy already existed for this SiteInstance.";
  RenderFrameProxyHost* proxy_host =
      new RenderFrameProxyHost(site_instance, rvh, frame_tree_node_);
  proxy_hosts_[site_instance_id] = base::WrapUnique(proxy_host);
  static_cast<SiteInstanceImpl*>(site_instance)->AddObserver(this);
  return proxy_host;
}
