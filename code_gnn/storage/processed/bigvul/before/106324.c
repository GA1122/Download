SyncBackendHost::StatusSummary SyncBackendHost::GetStatusSummary() {
  DCHECK(syncapi_initialized_);
  return core_->syncapi()->GetStatusSummary();
}
