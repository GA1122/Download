void Document::BeginLifecycleUpdatesIfRenderingReady() {
  if (!IsActive())
    return;
  if (!HaveRenderBlockingResourcesLoaded())
    return;
  View()->BeginLifecycleUpdates();
}
