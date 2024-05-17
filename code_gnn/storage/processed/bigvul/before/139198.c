void RenderProcessHostImpl::RecomputeAndUpdateWebKitPreferences() {
  std::unique_ptr<RenderWidgetHostIterator> widgets(
      RenderWidgetHostImpl::GetAllRenderWidgetHosts());
  while (RenderWidgetHost* widget = widgets->GetNextHost()) {
    RenderViewHost* rvh = RenderViewHost::From(widget);
    if (!rvh)
      continue;

    if (rvh->GetProcess()->GetID() != GetID())
      continue;

    rvh->OnWebkitPreferencesChanged();
  }
}
