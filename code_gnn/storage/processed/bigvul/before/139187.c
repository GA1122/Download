void RenderProcessHostImpl::OnGpuSwitched() {
  RecomputeAndUpdateWebKitPreferences();
}