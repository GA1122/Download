void SyncManager::SyncInternal::UpdateCryptographerAndNigori(
    const std::string& chrome_version,
    const base::Closure& done_callback) {
  DCHECK(initialized_);
  browser_sync::GetSessionName(
      blocking_task_runner_,
      base::Bind(
          &SyncManager::SyncInternal::UpdateCryptographerAndNigoriCallback,
          weak_ptr_factory_.GetWeakPtr(),
          chrome_version,
          done_callback));
}
