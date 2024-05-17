void SyncManager::EnableEncryptEverything() {
  DCHECK(thread_checker_.CalledOnValidThread());
  {
    WriteTransaction trans(FROM_HERE, GetUserShare());
    Cryptographer* cryptographer = trans.GetCryptographer();
    if (cryptographer->is_ready())
      cryptographer->set_encrypt_everything();
  }

  data_->RefreshEncryption();
}
