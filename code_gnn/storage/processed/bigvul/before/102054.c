bool SyncBackendHost::EncryptEverythingEnabled() const {
  if (initialization_state_ == NOT_INITIALIZED) {
    NOTREACHED() << "Cannot check encryption status without first "
                 << "initializing backend.";
    return false;
  }
  return core_->sync_manager()->EncryptEverythingEnabled();
}
