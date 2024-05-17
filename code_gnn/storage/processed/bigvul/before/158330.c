void RenderWidgetHostImpl::OnSnapshotReceived(int snapshot_id,
                                              gfx::Image image) {
  PendingSnapshotMap::iterator it = pending_browser_snapshots_.begin();
  while (it != pending_browser_snapshots_.end()) {
    if (it->first <= snapshot_id) {
      it->second.Run(image);
      pending_browser_snapshots_.erase(it++);
    } else {
      ++it;
    }
  }
#if defined(OS_MACOSX)
  if (pending_browser_snapshots_.empty())
    GetWakeLock()->CancelWakeLock();
#endif
}
