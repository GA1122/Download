void SyncManager::SyncInternal::FinishSetPassphrase(
    bool success,
    const std::string& bootstrap_token,
    bool is_explicit,
    WriteTransaction* trans,
    WriteNode* nigori_node) {
  Cryptographer* cryptographer = trans->GetCryptographer();
  NotifyCryptographerState(cryptographer);

  if (!bootstrap_token.empty()) {
    DVLOG(1) << "Bootstrap token updated.";
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnBootstrapTokenUpdated(bootstrap_token));
  }

  if (!success) {
    if (cryptographer->is_ready()) {
      LOG(ERROR) << "Attempt to change passphrase failed while cryptographer "
                 << "was ready.";
    } else if (cryptographer->has_pending_keys()) {
      FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                        OnPassphraseRequired(sync_api::REASON_DECRYPTION,
                                             cryptographer->GetPendingKeys()));
    } else {
      FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                        OnPassphraseRequired(sync_api::REASON_ENCRYPTION,
                                             sync_pb::EncryptedData()));
    }
    return;
  }

  FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                    OnPassphraseAccepted());
  DCHECK(cryptographer->is_ready());

  sync_pb::NigoriSpecifics specifics(nigori_node->GetNigoriSpecifics());
  if (!cryptographer->GetKeys(specifics.mutable_encrypted())) {
    NOTREACHED();
    return;
  }
  specifics.set_using_explicit_passphrase(is_explicit);
  nigori_node->SetNigoriSpecifics(specifics);

  ReEncryptEverything(trans);
}
