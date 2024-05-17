void SyncManager::SyncInternal::UpdateNigoriEncryptionState(
    Cryptographer* cryptographer,
    WriteNode* nigori_node) {
  DCHECK(nigori_node);
  sync_pb::NigoriSpecifics nigori = nigori_node->GetNigoriSpecifics();

  if (cryptographer->is_ready() &&
      nigori_overwrite_count_ < kNigoriOverwriteLimit) {
    sync_pb::EncryptedData original_keys = nigori.encrypted();
    if (!cryptographer->GetKeys(nigori.mutable_encrypted()))
      NOTREACHED();

    if (nigori.encrypted().SerializeAsString() !=
        original_keys.SerializeAsString()) {
      nigori_overwrite_count_++;
      UMA_HISTOGRAM_COUNTS("Sync.AutoNigoriOverwrites",
                           nigori_overwrite_count_);
    }

  }
  cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);

  nigori_node->SetNigoriSpecifics(nigori);
}
