void SyncManager::SetEncryptionPassphrase(const std::string& passphrase,
                                          bool is_explicit) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->SetEncryptionPassphrase(passphrase, is_explicit);
}
