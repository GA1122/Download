void SyncBackendHost::Core::OnSyncCycleCompleted(
    const SyncSessionSnapshot& snapshot) {
  if (!sync_loop_)
    return;
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::HandleSyncCycleCompletedOnFrontendLoop,
      snapshot);
}
