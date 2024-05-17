void WebContentsImpl::RenderFrameCreated(RenderFrameHost* render_frame_host) {
  for (auto& observer : observers_)
    observer.RenderFrameCreated(render_frame_host);
  UpdateAccessibilityModeOnFrame(render_frame_host);

  if (!render_frame_host->IsRenderFrameLive() || render_frame_host->GetParent())
    return;

  NavigationEntry* entry = controller_.GetPendingEntry();
  if (entry && entry->IsViewSourceMode()) {
    render_frame_host->Send(
        new FrameMsg_EnableViewSourceMode(render_frame_host->GetRoutingID()));
  }
}
