void SyncManager::SyncInternal::EncryptDataTypes(
    const syncable::ModelTypeSet& encrypted_types) {
  DCHECK(initialized_);
  VLOG(1) << "Attempting to encrypt datatypes "
          << syncable::ModelTypeSetToString(encrypted_types);

  WriteTransaction trans(FROM_HERE, GetUserShare());
  WriteNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    NOTREACHED() << "Unable to set encrypted datatypes because Nigori node not "
                 << "found.";
    return;
  }

  Cryptographer* cryptographer = trans.GetCryptographer();

  if (!cryptographer->is_ready()) {
    VLOG(1) << "Attempting to encrypt datatypes when cryptographer not "
            << "initialized, prompting for passphrase.";
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnPassphraseRequired(sync_api::REASON_DECRYPTION));
    return;
  }

  cryptographer->SetEncryptedTypes(encrypted_types);
  sync_pb::NigoriSpecifics nigori;
  nigori.CopyFrom(node.GetNigoriSpecifics());
  cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);
  node.SetNigoriSpecifics(nigori);
  allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

  ReEncryptEverything(&trans);
  return;
}
