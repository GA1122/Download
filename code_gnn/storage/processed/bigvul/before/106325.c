sync_api::UserShare* SyncBackendHost::GetUserShare() const {
  DCHECK(syncapi_initialized_);
  return core_->syncapi()->GetUserShare();
}
