void SyncManager::SyncInternal::SetPassphrase(
    const std::string& passphrase, bool is_explicit) {
  if (passphrase.empty()) {
    VLOG(1) << "Rejecting empty passphrase.";
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
        OnPassphraseRequired(sync_api::REASON_SET_PASSPHRASE_FAILED));
    return;
  }

  WriteTransaction trans(FROM_HERE, GetUserShare());
  Cryptographer* cryptographer = trans.GetCryptographer();
  KeyParams params = {"localhost", "dummy", passphrase};

  WriteNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    NOTREACHED();
    return;
  }

  if (cryptographer->has_pending_keys()) {
    bool succeeded = false;

    if (node.GetNigoriSpecifics().using_explicit_passphrase() == is_explicit) {
      if (cryptographer->DecryptPendingKeys(params)) {
        succeeded = true;
      } else {
        VLOG(1) << "Passphrase failed to decrypt pending keys.";
      }
    } else {
      VLOG(1) << "Not trying the passphrase because the explicit flags dont "
              << "match. Nigori node's explicit flag is "
              << node.GetNigoriSpecifics().using_explicit_passphrase();
    }

    if (!succeeded) {
      ObserverList<SyncManager::Observer> temp_obs_list;
      CopyObservers(&temp_obs_list);
      FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
          OnPassphraseRequired(sync_api::REASON_SET_PASSPHRASE_FAILED));
      return;
    }

    RequestNudge(FROM_HERE);
  } else {
    VLOG(1) << "No pending keys, adding provided passphrase.";

    if (!is_explicit && node.GetNigoriSpecifics().using_explicit_passphrase())
      return;

    cryptographer->AddKey(params);

    sync_pb::NigoriSpecifics specifics(node.GetNigoriSpecifics());
    specifics.clear_encrypted();
    cryptographer->GetKeys(specifics.mutable_encrypted());
    specifics.set_using_explicit_passphrase(is_explicit);
    node.SetNigoriSpecifics(specifics);
  }

  ReEncryptEverything(&trans);

  VLOG(1) << "Passphrase accepted, bootstrapping encryption.";
  std::string bootstrap_token;
  cryptographer->GetBootstrapToken(&bootstrap_token);
  ObserverList<SyncManager::Observer> temp_obs_list;
  CopyObservers(&temp_obs_list);
  FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                    OnPassphraseAccepted(bootstrap_token));
}
