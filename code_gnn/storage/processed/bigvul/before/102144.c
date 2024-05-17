void SyncManager::SyncInternal::HandleCalculateChangesChangeEventFromSyncApi(
    const EntryKernelMutationSet& mutations,
    syncable::BaseTransaction* trans) {
  if (!scheduler()) {
    return;
  }

  LOG_IF(WARNING, !ChangeBuffersAreEmpty()) <<
      "CALCULATE_CHANGES called with unapplied old changes.";

  syncable::ModelType mutated_model_type = syncable::UNSPECIFIED;

  for (syncable::EntryKernelMutationSet::const_iterator it =
           mutations.begin(); it != mutations.end(); ++it) {
    if (!it->mutated.ref(syncable::IS_UNSYNCED)) {
      continue;
    }

    syncable::ModelType model_type =
        syncable::GetModelTypeFromSpecifics(it->mutated.ref(SPECIFICS));
    if (model_type < syncable::FIRST_REAL_MODEL_TYPE) {
      NOTREACHED() << "Permanent or underspecified item changed via syncapi.";
      continue;
    }

    if (mutated_model_type == syncable::UNSPECIFIED) {
      mutated_model_type = model_type;
      break;
    }
  }

  if (mutated_model_type != syncable::UNSPECIFIED) {
    if (weak_handle_this_.IsInitialized()) {
      weak_handle_this_.Call(FROM_HERE,
                             &SyncInternal::RequestNudgeForDataType,
                             FROM_HERE,
                             mutated_model_type);
    } else {
      NOTREACHED();
    }
  }
}
