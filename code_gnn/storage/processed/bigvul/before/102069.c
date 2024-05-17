void SyncBackendHost::HandleInitializationCompletedOnFrontendLoop(
    const WeakHandle<JsBackend>& js_backend, bool success) {
  if (!frontend_)
    return;
  if (!success) {
    frontend_->OnBackendInitialized(WeakHandle<JsBackend>(), false);
    return;
  }

  bool setup_completed =
      profile_->GetPrefs()->GetBoolean(prefs::kSyncHasSetupCompleted);
  if (setup_completed || initialization_state_ == DOWNLOADING_NIGORI) {
    if (setup_completed)
      core_->sync_manager()->RefreshEncryption();
    initialization_state_ = INITIALIZED;
    frontend_->OnBackendInitialized(js_backend, true);
    return;
  }

  initialization_state_ = DOWNLOADING_NIGORI;
  ConfigureDataTypes(
      syncable::ModelTypeSet(),
      syncable::ModelTypeSet(),
      sync_api::CONFIGURE_REASON_NEW_CLIENT,
      base::Bind(
          &SyncBackendHost::Core::HandleInitializationCompletedOnFrontendLoop,
          core_.get(), js_backend),
      true);
}
