PageLoadTracker* MetricsWebContentsObserver::GetTrackerOrNullForRequest(
    const content::GlobalRequestID& request_id,
    content::RenderFrameHost* render_frame_host_or_null,
    content::ResourceType resource_type,
    base::TimeTicks creation_time) {
  if (resource_type == content::RESOURCE_TYPE_MAIN_FRAME) {
    DCHECK(request_id != content::GlobalRequestID());
    for (const auto& kv : provisional_loads_) {
      PageLoadTracker* candidate = kv.second.get();
      if (candidate->HasMatchingNavigationRequestID(request_id)) {
        return candidate;
      }
    }
    if (committed_load_ &&
        committed_load_->HasMatchingNavigationRequestID(request_id)) {
      return committed_load_.get();
    }
  } else {
    if (!committed_load_ || creation_time < committed_load_->navigation_start())
      return nullptr;

    if (resource_type == content::RESOURCE_TYPE_SUB_FRAME)
      return committed_load_.get();

    if (!render_frame_host_or_null)
      return nullptr;

    content::RenderFrameHost* main_frame_for_resource =
        GetMainFrame(render_frame_host_or_null);
    if (main_frame_for_resource == web_contents()->GetMainFrame())
      return committed_load_.get();
  }
  return nullptr;
}
