ModelTypeBitSet SyncManager::SyncInternal::HandleTransactionEndingChangeEvent(
    syncable::BaseTransaction* trans) {
  if (!HaveObservers() || ChangeBuffersAreEmpty())
    return ModelTypeBitSet();

  ReadTransaction read_trans(GetUserShare(), trans);

  syncable::ModelTypeBitSet models_with_changes;
  for (int i = 0; i < syncable::MODEL_TYPE_COUNT; ++i) {
    if (change_buffers_[i].IsEmpty())
      continue;

    vector<ChangeRecord> ordered_changes;
    change_buffers_[i].GetAllChangesInTreeOrder(&read_trans, &ordered_changes);
    if (!ordered_changes.empty()) {
      ObserverList<SyncManager::Observer> temp_obs_list;
      CopyObservers(&temp_obs_list);
      FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
          OnChangesApplied(syncable::ModelTypeFromInt(i), &read_trans,
                           &ordered_changes[0], ordered_changes.size()));
      models_with_changes.set(i, true);
    }
    change_buffers_[i].Clear();
  }
  return models_with_changes;
}
