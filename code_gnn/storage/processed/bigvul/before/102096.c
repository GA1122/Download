void SyncBackendHost::Core::OnSyncCycleCompleted(
    const SyncSessionSnapshot* snapshot) {
  host_->frontend_loop_->PostTask(FROM_HERE, NewRunnableMethod(this,
      &Core::HandleSyncCycleCompletedOnFrontendLoop,
      new SyncSessionSnapshot(*snapshot)));
}
