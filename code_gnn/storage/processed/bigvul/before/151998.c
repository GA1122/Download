void RenderFrameHostImpl::LeaveBackForwardCache() {
  DCHECK(IsBackForwardCacheEnabled());
  DCHECK(is_in_back_forward_cache_);
  is_in_back_forward_cache_ = false;
  for (auto& child : children_)
    child->current_frame_host()->LeaveBackForwardCache();
}
