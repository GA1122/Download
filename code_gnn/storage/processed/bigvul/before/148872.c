bool RenderFrameHostManager::DeleteFromPendingList(
    RenderFrameHostImpl* render_frame_host) {
  for (RFHPendingDeleteList::iterator iter = pending_delete_hosts_.begin();
       iter != pending_delete_hosts_.end();
       iter++) {
    if (iter->get() == render_frame_host) {
      pending_delete_hosts_.erase(iter);
      return true;
    }
  }
  return false;
}
