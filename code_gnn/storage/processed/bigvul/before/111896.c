bool ProfileSyncService::IsEncryptedDatatypeEnabled() const {
  if (encryption_pending())
    return true;
  const syncable::ModelTypeSet preferred_types = GetPreferredDataTypes();
  const syncable::ModelTypeSet encrypted_types = GetEncryptedDataTypes();
  DCHECK(encrypted_types.Has(syncable::PASSWORDS));
  return !Intersection(preferred_types, encrypted_types).Empty();
}
