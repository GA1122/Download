bool SessionModelAssociator::CryptoReadyIfNecessary() {
  sync_api::ReadTransaction trans(FROM_HERE, sync_service_->GetUserShare());
  syncable::ModelTypeSet encrypted_types;
  encrypted_types = sync_api::GetEncryptedTypes(&trans);
  return encrypted_types.count(syncable::SESSIONS) == 0 ||
         sync_service_->IsCryptographerReady(&trans);
}
