void RenderProcessHostImpl::AddPendingView() {
  pending_views_++;
  UpdateProcessPriority();
}
