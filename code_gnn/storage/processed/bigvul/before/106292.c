void SyncBackendHost::ConfigureAutofillMigration() {
  if (GetAutofillMigrationState() == syncable::NOT_DETERMINED) {
    sync_api::ReadTransaction trans(GetUserShare());
    sync_api::ReadNode autofil_root_node(&trans);

    if (!autofil_root_node.InitByTagLookup(browser_sync::kAutofillTag)) {
        SetAutofillMigrationState(syncable::INSUFFICIENT_INFO_TO_DETERMINE);
      return;
    }

    if (autofil_root_node.GetFirstChildId() == static_cast<int64>(0)) {
      SetAutofillMigrationState(syncable::INSUFFICIENT_INFO_TO_DETERMINE);
      return;
    }

    sync_api::ReadNode autofill_profile_root_node(&trans);

    if (!autofill_profile_root_node.InitByTagLookup(
       browser_sync::kAutofillProfileTag)) {
      SetAutofillMigrationState(syncable::NOT_MIGRATED);
      return;
    }

    DCHECK(false);

    SetAutofillMigrationState(syncable::NOT_MIGRATED);
    return;
  }
}
