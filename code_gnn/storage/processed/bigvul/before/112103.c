void SyncManager::RefreshNigori(const std::string& chrome_version,
                                const base::Closure& done_callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->UpdateCryptographerAndNigori(
      chrome_version,
      done_callback);
}
