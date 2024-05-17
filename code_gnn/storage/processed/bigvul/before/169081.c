void OfflinePageModelImpl::OnEnsureArchivesDirCreatedDone(
    const base::TimeTicks& start_time) {
  UMA_HISTOGRAM_TIMES("OfflinePages.Model.ArchiveDirCreationTime",
                      base::TimeTicks::Now() - start_time);

  store_->Initialize(base::Bind(&OfflinePageModelImpl::OnStoreInitialized,
                                weak_ptr_factory_.GetWeakPtr(), start_time, 0));
}
