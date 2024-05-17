void OfflinePageModelImpl::RetryDbInitialization(
    const base::TimeTicks& start_time,
    int init_attempts_spent) {
  store_->Initialize(base::Bind(&OfflinePageModelImpl::OnStoreInitialized,
                                weak_ptr_factory_.GetWeakPtr(), start_time,
                                init_attempts_spent));
}
