void SyncManager::SetDecryptionPassphrase(const std::string& passphrase) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->SetDecryptionPassphrase(passphrase);
}
