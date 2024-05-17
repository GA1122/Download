RenderFrameProxyHost* RenderFrameHostManager::GetRenderFrameProxyHost(
    SiteInstance* instance) const {
  auto it = proxy_hosts_.find(instance->GetId());
  if (it != proxy_hosts_.end())
    return it->second.get();
  return nullptr;
}
