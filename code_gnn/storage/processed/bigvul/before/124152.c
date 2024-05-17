RenderViewHostImpl* RenderViewHostManager::GetSwappedOutRenderViewHost(
    SiteInstance* instance) {
  RenderViewHostMap::iterator iter = swapped_out_hosts_.find(instance->GetId());
  if (iter != swapped_out_hosts_.end())
    return iter->second;

  return NULL;
}
