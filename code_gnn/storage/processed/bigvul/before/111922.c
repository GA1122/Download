void ProfileSyncService::OnUserChoseDatatypes(bool sync_everything,
    syncable::ModelTypeSet chosen_types) {
  if (!backend_.get() &&
      unrecoverable_error_detected_ == false) {
    NOTREACHED();
    return;
  }

  UpdateSelectedTypesHistogram(sync_everything, chosen_types);
  sync_prefs_.SetKeepEverythingSynced(sync_everything);

  failed_datatypes_handler_.OnUserChoseDatatypes();
  ChangePreferredDataTypes(chosen_types);
  AcknowledgeSyncedTypes();
  NotifyObservers();
}
