void RenderWidgetHostImpl::GetSnapshotFromBrowser(
    const GetSnapshotFromBrowserCallback& callback,
    bool from_surface) {
  int snapshot_id = next_browser_snapshot_id_++;
  if (from_surface) {
    pending_surface_browser_snapshots_.insert(
        std::make_pair(snapshot_id, callback));
    Send(new ViewMsg_ForceRedraw(GetRoutingID(), snapshot_id));
    return;
  }

#if defined(OS_MACOSX)
  if (pending_browser_snapshots_.empty())
    GetWakeLock()->RequestWakeLock();
#endif
  pending_browser_snapshots_.insert(std::make_pair(snapshot_id, callback));
  Send(new ViewMsg_ForceRedraw(GetRoutingID(), snapshot_id));
}
