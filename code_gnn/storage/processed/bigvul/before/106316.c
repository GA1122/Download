string16 SyncBackendHost::GetAuthenticatedUsername() const {
  DCHECK(syncapi_initialized_);
  return UTF8ToUTF16(core_->syncapi()->GetAuthenticatedUsername());
}
