bool RenderViewHostManager::IsSwappedOut(RenderViewHost* rvh) {
  if (!rvh->GetSiteInstance())
    return false;

  return swapped_out_hosts_.find(rvh->GetSiteInstance()->GetId()) !=
      swapped_out_hosts_.end();
}
