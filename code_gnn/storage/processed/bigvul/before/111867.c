void ProfileSyncService::ConsumeCachedPassphraseIfPossible() {
  if (cached_passphrase_.empty() || !sync_initialized())
    return;

  std::string passphrase = cached_passphrase_;
  cached_passphrase_.clear();

  if (passphrase_required_reason() == sync_api::REASON_DECRYPTION) {
    if (SetDecryptionPassphrase(passphrase)) {
      DVLOG(1) << "Cached passphrase successfully decrypted pending keys";
      return;
    }
  }

  if (!IsUsingSecondaryPassphrase())
    SetEncryptionPassphrase(passphrase, IMPLICIT);
}
