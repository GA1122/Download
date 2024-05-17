bool SyncBackendHost::IsCryptographerReady(
    const sync_api::BaseTransaction* trans) const {
  return syncapi_initialized_ && trans->GetCryptographer()->is_ready();
}
