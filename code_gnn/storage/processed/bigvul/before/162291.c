void CommandBufferProxyImpl::SetSnapshotRequested() {
  CheckLock();
  snapshot_requested_ = true;
}
