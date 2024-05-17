bool ProfileSyncService::IsPassphraseRequiredForDecryption() const {
  return IsEncryptedDatatypeEnabled() && IsPassphraseRequired();
}
