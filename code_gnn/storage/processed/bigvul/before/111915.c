void ProfileSyncService::OnPassphraseRequired(
    sync_api::PassphraseRequiredReason reason,
    const sync_pb::EncryptedData& pending_keys) {
  DCHECK(backend_.get());
  DCHECK(backend_->IsNigoriEnabled());

  if (unrecoverable_error_detected_) {
    return;
  }

  DVLOG(1) << "Passphrase required with reason: "
           << sync_api::PassphraseRequiredReasonToString(reason);
  passphrase_required_reason_ = reason;

  NotifyObservers();
}
