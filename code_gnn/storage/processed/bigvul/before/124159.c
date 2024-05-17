void RenderViewHostManager::RenderViewDeleted(RenderViewHost* rvh) {
  if (rvh == pending_render_view_host_) {
    NOTREACHED();
    pending_render_view_host_ = NULL;
  }

  if (!render_view_host_)
    return;
  for (RenderViewHostMap::iterator iter = swapped_out_hosts_.begin();
       iter != swapped_out_hosts_.end();
       ++iter) {
    if (iter->second == rvh) {
      swapped_out_hosts_.erase(iter);
      break;
    }
  }
}
