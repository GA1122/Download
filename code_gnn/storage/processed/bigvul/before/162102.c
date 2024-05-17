bool RenderProcessHostImpl::HostHasNotBeenUsed() {
  return IsUnused() && listeners_.IsEmpty() && keep_alive_ref_count_ == 0 &&
         pending_views_ == 0;
}
