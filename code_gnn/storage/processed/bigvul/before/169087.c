void OfflinePageModelImpl::OnStoreInitialized(const base::TimeTicks& start_time,
                                              int init_attempts_spent,
                                              bool success) {
  init_attempts_spent++;

  if (success) {
    DCHECK_EQ(store_->state(), StoreState::LOADED);
    ReportInitializationAttemptsSpent(init_attempts_spent);
    store_->GetOfflinePages(
        base::Bind(&OfflinePageModelImpl::OnInitialGetOfflinePagesDone,
                   weak_ptr_factory_.GetWeakPtr(), start_time));
    return;
  }

  DCHECK_EQ(store_->state(), StoreState::FAILED_LOADING);
  if (init_attempts_spent >= kInitializeAttemptsMax) {
    FinalizeModelLoad();
    return;
  }

  const base::TimeDelta delay = base::TimeDelta::FromMilliseconds(100);
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&OfflinePageModelImpl::RetryDbInitialization,
                 weak_ptr_factory_.GetWeakPtr(), start_time,
                 init_attempts_spent),
      delay);
}
