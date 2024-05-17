void SyncManager::SyncInternal::HandleTransactionCompleteChangeEvent(
    const syncable::ModelTypeBitSet& models_with_changes) {
  if (!HaveObservers())
    return;

  for (int i = 0; i < syncable::MODEL_TYPE_COUNT; ++i) {
    if (models_with_changes.test(i)) {
      ObserverList<SyncManager::Observer> temp_obs_list;
      CopyObservers(&temp_obs_list);
      FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                        OnChangesComplete(syncable::ModelTypeFromInt(i)));
    }
  }
}
