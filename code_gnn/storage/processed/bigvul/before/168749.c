void RenderWidgetHostImpl::GetSnapshotFromBrowser(
    const GetSnapshotFromBrowserCallback& callback,
    bool from_surface) {
  int id = next_browser_snapshot_id_++;
  if (from_surface) {
    pending_surface_browser_snapshots_.insert(std::make_pair(id, callback));
    ui::LatencyInfo latency_info;
    latency_info.AddLatencyNumber(ui::BROWSER_SNAPSHOT_FRAME_NUMBER_COMPONENT,
                                  0, id);
    Send(new ViewMsg_ForceRedraw(GetRoutingID(), latency_info));
    return;
  }

#if defined(OS_MACOSX)
  if (pending_browser_snapshots_.empty())
    GetWakeLock()->RequestWakeLock();
#endif
  pending_browser_snapshots_.insert(std::make_pair(id, callback));
  ui::LatencyInfo latency_info;
  latency_info.AddLatencyNumber(ui::BROWSER_SNAPSHOT_FRAME_NUMBER_COMPONENT, 0,
                                id);
  Send(new ViewMsg_ForceRedraw(GetRoutingID(), latency_info));
}
