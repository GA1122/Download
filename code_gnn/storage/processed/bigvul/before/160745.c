void RenderFrameImpl::UpdateZoomLevel() {
  if (!frame_->Parent()) {
    render_view_->webview()->ZoomLimitsChanged(
        ZoomFactorToZoomLevel(kMinimumZoomFactor),
        ZoomFactorToZoomLevel(kMaximumZoomFactor));

    HostZoomLevels::iterator host_zoom =
        host_zoom_levels_.find(GetLoadingUrl());
    if (render_view_->webview()->MainFrame()->IsWebLocalFrame() &&
        render_view_->webview()
            ->MainFrame()
            ->ToWebLocalFrame()
            ->GetDocument()
            .IsPluginDocument()) {
      render_view_->SetZoomLevel(0);
    } else {
      if (host_zoom != host_zoom_levels_.end())
        render_view_->SetZoomLevel(host_zoom->second);
    }

    if (host_zoom != host_zoom_levels_.end()) {
      host_zoom_levels_.erase(host_zoom);
    }
  } else {
    render_view_->SetZoomLevel(render_view_->page_zoom_level());
  }
}
