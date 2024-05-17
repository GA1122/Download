void SyncBackendHost::Core::DeferNudgeForCleanup() {
  DCHECK_EQ(MessageLoop::current(), host_->core_thread_.message_loop());
  deferred_nudge_for_cleanup_requested_ = true;
}
