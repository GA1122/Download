void ProfileSyncService::OnBackendInitialized(
    const WeakHandle<JsBackend>& js_backend, bool success) {
  if (!HasSyncSetupCompleted()) {
    UMA_HISTOGRAM_BOOLEAN("Sync.BackendInitializeFirstTimeSuccess", success);
  } else {
    UMA_HISTOGRAM_BOOLEAN("Sync.BackendInitializeRestoreSuccess", success);
  }

  if (!success) {
    OnUnrecoverableErrorImpl(FROM_HERE, "BackendInitialize failure", false);
    return;
  }

  backend_initialized_ = true;

  sync_js_controller_.AttachJsBackend(js_backend);

  ConsumeCachedPassphraseIfPossible();

  if (last_synced_time_.is_null()) {
    UpdateLastSyncedTime();
  }
  NotifyObservers();

  if (auto_start_enabled_ && !FirstSetupInProgress()) {
    SetSyncSetupCompleted();
    NotifyObservers();
  }

  if (HasSyncSetupCompleted()) {
    ConfigureDataTypeManager();
  } else {
    DCHECK(FirstSetupInProgress());
  }
}
