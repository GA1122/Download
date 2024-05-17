bool SyncManager::SyncInternal::UpdateCryptographerFromNigori() {
  DCHECK(initialized_);
  syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
  if (!lookup.good()) {
    NOTREACHED() << "BootstrapEncryption: lookup not good so bailing out";
    return false;
  }
  if (!lookup->initial_sync_ended_for_type(syncable::NIGORI))
    return false;   

  WriteTransaction trans(FROM_HERE, GetUserShare());
  Cryptographer* cryptographer = trans.GetCryptographer();

  WriteNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    NOTREACHED();
    return false;
  }
  sync_pb::NigoriSpecifics nigori(node.GetNigoriSpecifics());
  Cryptographer::UpdateResult result = cryptographer->Update(nigori);
  if (result == Cryptographer::NEEDS_PASSPHRASE) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnPassphraseRequired(sync_api::REASON_DECRYPTION));
  }

  cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);
  node.SetNigoriSpecifics(nigori);

  allstatus_.SetCryptographerReady(cryptographer->is_ready());
  allstatus_.SetCryptoHasPendingKeys(cryptographer->has_pending_keys());
  allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

  return cryptographer->is_ready();
}
