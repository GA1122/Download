void ProfileSyncService::OnPassphraseAccepted() {
  DVLOG(1) << "Received OnPassphraseAccepted.";
  DCHECK(cached_passphrase_.empty()) <<
      "Passphrase no longer required but there is still a cached passphrase";

  passphrase_required_reason_ = sync_api::REASON_PASSPHRASE_NOT_REQUIRED;

  const syncable::ModelTypeSet types = GetPreferredDataTypes();

  if (data_type_manager_.get()) {
    data_type_manager_->Configure(types,
                                  sync_api::CONFIGURE_REASON_RECONFIGURATION);
  }

  NotifyObservers();
}
