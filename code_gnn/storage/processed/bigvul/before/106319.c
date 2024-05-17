SyncBackendHost::Status SyncBackendHost::GetDetailedStatus() {
  DCHECK(syncapi_initialized_);
  return core_->syncapi()->GetDetailedStatus();
}
