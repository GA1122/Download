void SyncManager::SyncInternal::HandleTransactionCompleteChangeEvent(
    ModelTypeSet models_with_changes) {
  if (!change_delegate_)
    return;

  for (ModelTypeSet::Iterator it = models_with_changes.First();
       it.Good(); it.Inc()) {
    change_delegate_->OnChangesComplete(it.Get());
    change_observer_.Call(
        FROM_HERE, &SyncManager::ChangeObserver::OnChangesComplete, it.Get());
  }
}
