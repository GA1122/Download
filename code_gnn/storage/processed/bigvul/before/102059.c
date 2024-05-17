string16 SyncBackendHost::GetAuthenticatedUsername() const {
  DCHECK(initialized());
  return UTF8ToUTF16(core_->sync_manager()->GetAuthenticatedUsername());
}
