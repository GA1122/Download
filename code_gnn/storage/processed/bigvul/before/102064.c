SyncBackendHost::StatusSummary SyncBackendHost::GetStatusSummary() {
  DCHECK(initialized());
  return core_->sync_manager()->GetStatusSummary();
}
