bool RenderProcessHostImpl::MayReuseHost() {
  if (is_never_suitable_for_reuse_)
    return false;

  return GetContentClient()->browser()->MayReuseHost(this);
}
