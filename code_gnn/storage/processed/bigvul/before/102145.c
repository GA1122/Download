void SyncManager::SyncInternal::HandleCalculateChangesChangeEventFromSyncer(
    const EntryKernelMutationSet& mutations,
    syncable::BaseTransaction* trans) {
  LOG_IF(WARNING, !ChangeBuffersAreEmpty()) <<
      "CALCULATE_CHANGES called with unapplied old changes.";

  Cryptographer* crypto = dir_manager()->GetCryptographer(trans);
  for (syncable::EntryKernelMutationSet::const_iterator it =
           mutations.begin(); it != mutations.end(); ++it) {
    bool existed_before = !it->original.ref(syncable::IS_DEL);
    bool exists_now = !it->mutated.ref(syncable::IS_DEL);

    syncable::ModelType type =
        syncable::GetModelTypeFromSpecifics(it->mutated.ref(SPECIFICS));
    if (type < syncable::FIRST_REAL_MODEL_TYPE)
      continue;

    int64 id = it->original.ref(syncable::META_HANDLE);
    if (exists_now && !existed_before)
      change_buffers_[type].PushAddedItem(id);
    else if (!exists_now && existed_before)
      change_buffers_[type].PushDeletedItem(id);
    else if (exists_now && existed_before &&
             VisiblePropertiesDiffer(*it, crypto)) {
      change_buffers_[type].PushUpdatedItem(
          id, VisiblePositionsDiffer(*it));
    }

    SetExtraChangeRecordData(id, type, &change_buffers_[type], crypto,
                             it->original, existed_before, exists_now);
  }
}
