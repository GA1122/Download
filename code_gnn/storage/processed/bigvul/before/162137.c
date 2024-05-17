void RenderProcessHostImpl::RemovePendingView() {
  DCHECK(pending_views_);
  pending_views_--;
  UpdateProcessPriority();
}
