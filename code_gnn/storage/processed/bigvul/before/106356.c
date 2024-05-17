void SyncBackendHost::SetAutofillMigrationState(
    syncable::AutofillMigrationState state) {
  return core_->syncapi()->SetAutofillMigrationState(state);
}
