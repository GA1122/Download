void ProfileSyncService::SetEncryptionPassphrase(const std::string& passphrase,
                                                 PassphraseType type) {
  DCHECK(sync_initialized());
  DCHECK(!(type == IMPLICIT && IsUsingSecondaryPassphrase())) <<
      "Data is already encrypted using an explicit passphrase";
  DCHECK(!(type == EXPLICIT && IsPassphraseRequired())) <<
      "Cannot switch to an explicit passphrase if a passphrase is required";

  if (type == EXPLICIT)
    UMA_HISTOGRAM_BOOLEAN("Sync.CustomPassphrase", true);

  DVLOG(1) << "Setting " << (type == EXPLICIT ? "explicit" : "implicit")
           << " passphrase for encryption.";
  if (passphrase_required_reason_ == sync_api::REASON_ENCRYPTION) {
    passphrase_required_reason_ = sync_api::REASON_PASSPHRASE_NOT_REQUIRED;
    NotifyObservers();
  }
  backend_->SetEncryptionPassphrase(passphrase, type == EXPLICIT);
}
